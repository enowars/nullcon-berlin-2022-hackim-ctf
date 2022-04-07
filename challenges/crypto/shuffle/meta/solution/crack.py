#!/usr/bin/python3
from sympy.ntheory.modular import crt
from binascii import unhexlify
from Crypto.Cipher import AES

from secret import cycles, lcm
from chall import Permutation, pow

out = open('output','r').read().splitlines()
p = [int(x) for x in out[0].split(' = ')[1][1:-1].split(', ')]
A = [int(x) for x in out[1].split(' = ')[1][1:-1].split(', ')]
B = [int(x) for x in out[2].split(' = ')[1][1:-1].split(', ')]

def log(g,ga):
	system = []
	g_cyc = cycles(g)
	ga_cyc = cycles(ga)
	for ca in cycles(ga):
		for c in g_cyc:
			if ca[0] in c: break
		system.append(((c.index(ca[1]) - c.index(ca[0])) % len(c), len(c)))
	
	return crt([s[1] for s in system], [s[0] for s in system])[0]

a = log(p,A)
key = pow(Permutation(B), a).to_key()

cipher = AES.new(key, AES.MODE_CBC, bytes(16))
flag = cipher.decrypt(unhexlify(out[-1].split(' = ')[1]))
