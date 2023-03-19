# therealblue24's libc
what can i say? it's a c23-compliant libc implementation.

# NOTE
This libc is tested on MacOS. The Makefile for compiling may not work as it needs some tweaking.

# Contributing
Make a pull request with an explanation (detail) on what was added.

## Architectures supported
* aarch64 (arm64, M1, M2, ...)
I plan to implement amd64 support as well.

## Not Implemented for now
* `FILE` and some stdio function
* `wchar` functions
* `errno` support
* `getopt` support
* `time` support
* most of `stdlib` (WILL implement)
