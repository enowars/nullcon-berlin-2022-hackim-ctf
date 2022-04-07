#!/bin/sh

timeout --foreground 180 qemu-system-arm \
    -machine microbit \
    -device loader,file=/home/user/ctf.elf \
    -nographic

