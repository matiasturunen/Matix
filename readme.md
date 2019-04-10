# Matix operating system

## Getting started

Download and install qemu
Install arm build tools: `apt-get install gcc-arm-linux-gnueabi`

## Building arm

### assembly build

Do `make build` and then do `make run` to run the program

## QEMU

If not using QEMU VM, remove return from atag.c around line 5

## Getting it to run on the Pi

1. Flash memory card with actual rasbian image to create boot sectors
2. Delete all files from the card and copy `start.elf`, `kernel7.img`, `bootcode.bin` and `config.txt` to the card
3. Eject card and boot the Pi

## References
Boot code and parts of kernel: https://jsandler18.github.io/