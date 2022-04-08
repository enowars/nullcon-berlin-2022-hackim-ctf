#!/usr/bin/env python3

import itertools
from telnetlib import Telnet
from Crypto.Util.number import bytes_to_long, long_to_bytes
from tools import *

key = RSA.import_key(open('pubkey.pem','r').read())
msg = 'I love cookies.'

f = factorint(str_to_int(msg))
comb = itertools.product([0,1], repeat = len(f))
for row in comb:
	p = int(prod([list(f.keys())[i] for i in range(len(f)) if row[i] == 1]))
	q = int(prod([list(f.keys())[i] for i in range(len(f)) if row[i] == 0]))
	#print('%d * %d' % (p,q))
	l = [ord(c) for c in int_to_str(p)] + [ord(c) for c in int_to_str(q)]
	if min(l) >= 32:
		break

#adjust
io = Telnet('130.149.230.69',9122)

io.read_until(b'check]\n')
io.write(b'1:' + long_to_bytes(p) + b'\n')
io.read_until(b'\n')
foo = io.read_until(b'\n')
s1 = int(foo.strip().decode())
io.read_until(b'check]\n')
io.write(b'1:' + long_to_bytes(q) + b'\n')
io.read_until(b'\n')
s2 = int(io.read_until(b'\n').strip().decode())
io.read_until(b'check]\n')

signature = s1 * s2 % key.n
io.write(b'2:' + str(signature).encode() + b'\n')
io.read_until(b'\n')
flag = io.read_until(b'\n').decode()
print(flag)
