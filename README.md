# therealblue24's libc
What can I say? It's a C23-compliant libc implementation.

# NOTE
This libc is tested on MacOS. The Makefile for compiling the tests may not work as it needs some tweaking.

# Contributing
Make a pull request with an explanation (detail) on what was added.

## Architectures supported
* AArch64 (arm64, M1, M2, ...)
I plan to implement amd64 as well.

## Not Implemented for now
* `FILE` and some stdio function
* `wchar.h` functions
* `errno.h` support
* `getopt.h` support
* `time.h` support
* most of `stdlib.h` (WILL implement)
