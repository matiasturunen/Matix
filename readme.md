# Matix operating system

## Getting started

Download and install qemu
Install arm build tools: `apt-get install gcc-arm-linux-gnueabi`

## Building arm

### assembly build

Do `make build` and then do `make run` to run the program

## QEMU

If not using QEMU VM, remove return from atag.c around line 5

## References
Boot code and parts of kernel: https://jsandler18.github.io/