#!/bin/sh

timeout --foreground 180 qemu-system-x86_64 \
    -kernel /home/user/bzImage \
    -initrd /home/user/initramfs.cpio.gz \
    -cpu qemu64,+smep \
    -m 256M \
    -append "console=ttyS0 kaslr pti=on oops=panic panic=1 quiet" \
    -monitor /dev/null \
    -nographic \
    -no-reboot
