#!/usr/bin/python3
import random
from Crypto.Cipher import AES
from hashlib import sha256
from secret import flag, generate_base

class Permutation(object):
	def __init__(self, perm):
		self.perm = perm.copy()
		self.n = len(perm)
	
	def __mul__(self, perm):
		if self.n != perm.n:
			raise Exception
		return Permutation([perm.perm[self.perm[i]] for i in range(self.n)])

	def __str__(self):
		return str(self.perm)

	def to_key(self):
		b = 1
		while 256**b <= self.n:
			b += 1
		res = b''
		for x in self.perm:
			res += x.to_bytes(b,'big')
		return sha256(res).digest()

def pow(perm, k):
	if type(perm) == list: perm = Permutation(perm)
	res = Permutation(list(range(perm.n)))
	x = Permutation(perm.perm)
	if k == 0: return res
	while k > 1:
		if k & 1:
			res = x * res
		x = x * x
		k >>= 1
	return x * res

if __name__ == '__main__':
	p = generate_base(1<<16)
	#256 Bits of security should be enough for everyone
	a = random.randint(0, 1<<256)
	b = random.randint(0, 1<<256)
	A = pow(p,a)
	B = pow(p,b)
	writer = open('output','w')
	writer.write('p = %s\nA = %s\nB = %s\n' % (str(p), str(A), str(B)))
	key_Alice = pow(A,b).to_key()
	key_Bob = pow(B,a).to_key()
	assert key_Alice == key_Bob
	cipher = AES.new(key_Alice, AES.MODE_CBC, bytes(16))
	code = cipher.encrypt(flag.encode() + b'\x00' * (16 - len(flag) % 16))
	writer.write('enc_flag = %s' % code.hex())
	writer.close()

