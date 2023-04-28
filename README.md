# therealblue24's libc
what can i say? it's a c23-compliant libc implementation.

# NOTE
This libc is tested on MacOS. The Makefile for compiling may not work as it needs some tweaking.

# Contributing
Make a pull request with an explanation (detail) on what was added.

## Architectures supported
* aarch64 (arm64, M1, M2, ...)
* x86_64 (only 64-bit x86_64)
>

# Building
First, install any sort of C compiler and an assembler. Make sure you are running a unix system. After that, modify the `TARGET` variable in the Makefile to the target that your machine uses. Vaild targets are currently:
* `aarch64`
* `x86_64`
>
After that, run `$ make`. the output will be in the bin directory.
## NOTE
Currently the build target MUST be the architecture of your machine.
## NOTE 2
Only the `aarch64` architecture is tested because I have an M2 Mac, so if you experience any problems with architectures that arent `aarch64` then I probaly will just forward it to somebody else.

## Not Implemented for now
* `FILE` and some stdio function
* `wchar` functions
* `errno` support
* `getopt` support
* `time` support
* most of `stdlib` (WILL implement)

## Compatibilty Matrix (platform-dependant)
| Feature    | Compatible   | aarch64 | x86_64  |
| ---------- | ------------ | ------- | ------- |
| setjmp.h   | Yes          | Yes     | Yes     |
| CRT        | Yes*         | Yes     | Yes     |
| stdint.h   | Yes          | Yes     | Yes     |
| limits.h   | Yes          | Yes     | Yes     |
| Endianness | No           | No      | No      |
| fatptr_t   | Not Finished | No      | No      |

* CRT is partial for MacOS.
