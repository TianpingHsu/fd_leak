# Prologue
I came across a file descriptor leakage problem, so I rewrite "open", "close", "socket", "accept" functions to override the original ones in glibc.
When my application tries to open or close a file descriptor, I will save the calling stack and the I can print the fd usage information on a timer function.

# How To Use
## Build Shared Obejct
> g++ -shared -fPIC open_close_wrapper.cpp -o libopen_close_wrapper.so -ldl  


## Run Test Code
> g++ test.cpp -L ./ -lopen_close_wrapper  
> export LD_LIBRARY_PATH=./libopen_close_wrapper.so  
> ./a.out

# How To Parse The Backtrace
use the tool: 'addr2line'

P.S. You can add more infortion to the 'map', say 'thread id', 'time' etc.
