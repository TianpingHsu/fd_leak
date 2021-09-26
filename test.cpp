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

        // open files and print fd map info
        printf("now we open two files:\n");
        int fd1 = open_f1();
        int fd2 = open_f2();
        if (print_fd_info_map)
                print_fd_info_map();

        // close a fd and print fd map info
        printf("\nthen we close the first file:\n");
        close(fd1);
        if (print_fd_info_map)
                print_fd_info_map();

        // close a fd and print fd map info
        printf("\nthen we close the second file:\n");
        close(fd2);
        if (print_fd_info_map)
                print_fd_info_map();
        printf("\n");
        return 0;
}
