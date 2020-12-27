#include <dlfcn.h>
#include <stdio.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <map>
#include <string>


static std::map<int, std::string> fd_info_map;
static pthread_mutex_t* pmutex = NULL;
static bool mutex_inited = false;
static const int STACK_DEPTH = 40;

extern "C" void init_params()
{
        pmutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(pmutex, NULL);
}

void save_backtrace(int fd)
{
        int nptrs = 0;
        void *buffer[STACK_DEPTH];
        nptrs = backtrace(buffer, STACK_DEPTH);

        std::string bt;
        for (int i = 0; i < nptrs; i++) {
                char buf[32];
                snprintf(buf, sizeof(buf) - 1, "%p", buf);
                bt += buf;
                bt += " ";
        }

        pthread_mutex_lock(pmutex);
        fd_info_map[fd] = bt;
        pthread_mutex_unlock(pmutex);
}

extern "C" void print_fd_info_map()
{
        pthread_mutex_lock(pmutex);
        for (std::map<int, std::string>::iterator it = fd_info_map.begin(); it != fd_info_map.end(); it++) {
                printf("fd: %d, bt: %s\n", it->first, (it->second).c_str());
        }
        pthread_mutex_unlock(pmutex);
}

typedef int (*open_ptr)(const char*, int flags, ...);
extern "C" int open(const char* pathname, int flags, ...)
{
        int fd = -1;
        open_ptr ori_open = (open_ptr)dlsym(RTLD_NEXT, "open");

        if (flags & O_CREAT) {  // for more information read man page: man open
                va_list ap;
                va_start(ap, flags);
                mode_t mode = va_arg(ap, mode_t);
                fd = ori_open(pathname, flags, mode);
                va_end(ap);
        } else {
                fd = ori_open(pathname, flags);
        }
        //printf("open fd: %d\n", fd);

        save_backtrace(fd);
        return fd;
}

typedef int (*close_ptr)(int);
extern "C" int close(int fd)
{
        pthread_mutex_lock(pmutex);
        if (fd_info_map.find(fd) != fd_info_map.end()) {
                fd_info_map.erase(fd);
        }
        pthread_mutex_unlock(pmutex);

        //printf("close fd: %d\n", fd);
        close_ptr ori_close = (close_ptr)dlsym(RTLD_NEXT, "close");
        return ori_close(fd);
}

typedef int (*socket_ptr)(int, int, int);
extern "C" int socket(int domain, int type, int protocol)
{
        int fd = -1;
        socket_ptr ori_socket = (socket_ptr)dlsym(RTLD_NEXT, "socket");
        fd = ori_socket(domain, type, protocol);
        //printf("socket fd: %d\n", fd);
        save_backtrace(fd);
        return fd;
}

typedef int (*accept_ptr)(int, struct sockaddr*, socklen_t*);
extern "C" int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
        int fd = -1;
        accept_ptr ori_accept = (accept_ptr)dlsym(RTLD_NEXT, "accept");
        fd = ori_accept(sockfd, addr, addrlen);
        //printf("accept fd: %d\n", fd);
        save_backtrace(fd);
        return fd;
}
