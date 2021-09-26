# Prologue
I came across a file descriptor leakage problem, so I rewrite "open", "close", "socket", "accept" functions to override the original ones in glibc.

When my application tries to open or close a file descriptor, I will save the calling stack and then I can print the fd usage information on a timer function.


# How To Use
> make
> export LD_LIBRARY_PATH=./ ; ./test
here is the output:
```
now we open two files:
fd: 3, bt: 0x7fe0b0bafa14 0x7fe0b0bafd99 0x55ba4a5471c6 0x55ba4a547208 0x7fe0b066cb25 0x55ba4a5470de
fd: 4, bt: 0x7fe0b0bafa14 0x7fe0b0bafd99 0x55ba4a5471e5 0x55ba4a547210 0x7fe0b066cb25 0x55ba4a5470de

then we close the first file:
fd: 4, bt: 0x7fe0b0bafa14 0x7fe0b0bafd99 0x55ba4a5471e5 0x55ba4a547210 0x7fe0b066cb25 0x55ba4a5470de

then we close the second file:

```

# How To Parse The Backtrace
use the tool: 'addr2line'

P.S. You can add more infortion to the 'map', say 'thread id', 'time' etc.
