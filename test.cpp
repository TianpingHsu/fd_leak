#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
using namespace std;


int open_f1() {
        return open("open_close_wrapper.cpp", 0);
}

int open_f2() {
        return open("test.cpp", 0);
}

extern "C" void init_params();
extern "C" void print_fd_info_map();

int main ()
{
        init_params();
        int fd1 = open_f1();
        int fd2 = open_f2();
        if (print_fd_info_map)
                print_fd_info_map();
        printf("**************\n");

        close(fd1);
        printf("**************\n");
        if (print_fd_info_map)
                print_fd_info_map();

        close(fd2);
        printf("**************\n");
        if (print_fd_info_map)
                print_fd_info_map();

        return 0;
}
