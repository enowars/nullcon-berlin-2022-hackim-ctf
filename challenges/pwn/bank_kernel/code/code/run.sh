#!/bin/sh

LENGTH=9
STRENGTH=26
challenge=`dd bs=32 count=1 if=/dev/urandom 2>/dev/null | base64 | tr +/ _. | cut -c -$LENGTH`
echo hashcash -mb$STRENGTH $challenge

echo "hashcash token: "
read token
if [ `expr "$token" : "^[a-zA-Z0-9\+\_\.\:\/]\{52\}$"` -eq 52 ]; then
    hashcash -cdb$STRENGTH -f /tmp/hashcash.sdb -r $challenge $token 2> /dev/null
    if [ $? -eq 0 ]; then
        echo "[+] Correct"
    else
        echo "[-] Wrong"
        exit
    fi
else
    echo "[-] Wrong"
    exit
fi

timeout --foreground 180 qemu-system-x86_64 \
    -kernel /home/user/bzImage \
    -initrd /home/user/initramfs.cpio.gz \
    -cpu qemu64,+smep \
    -m 256M \
    -append "console=ttyS0 kaslr pti=on oops=panic panic=1 quiet" \
    -monitor /dev/null \
    -nographic \
    -no-reboot
