# Matix operating system

## Getting started

Download and install qemu
Install arm build tools: `apt-get install gcc-arm-linux-gnueabi`

## Building arm

### assembly build

Do `arm-linux-gnueabi-as -o helloworld.o helloworld.s` and then do `arm-linux-gnueabi-ld -o helloworld helloworld.o`

## References
Boot code and parts of kernel: https://jsandler18.github.io/