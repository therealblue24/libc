# therealblue24's libc
what can i say? it's a libc (c23-compliant) implementation.

# NOTE
this libc is tested on macos. the makefile for compiling the tests may not work as it needs tweaking.

# Contributing
Make a pull request with explanation (detail) on what you added.

## Architectures supported
* AArch64 (arm64, M1, M2, ...)
planning to do amd64

## Not Implemented for now
* `FILE` and some stdio function
* `wchar` functions
* `errno` support
* `getopt` support
* `time` support
* most of `stdlib` (WILL implement)
