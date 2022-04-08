#!/usr/bin/env sage

from Crypto.Util.number import bytes_to_long, long_to_bytes
from Crypto.PublicKey import RSA
from secret import flag
import sys

msg = 'I love cookies.'
key = RSA.importKey(open('privkey.pem','r').read())

def int_to_str(n):
	if n == 0:
		return ''
	return int_to_str(n // 256) + chr(n % 256)

def str_to_int(s):
	res = 0
	for char in s:
		res *= 256
		res += ord(char)
	return res

def sign(msg : bytes):
	#I will not talk about cookies.
	if b'cookie' in msg:
		return 0
	#no control characters allowed in message
	if any([c < 32 for c in msg]):
		return 0
	return pow(bytes_to_long(msg), key.d, key.n)

def verify(number : int):
	if str_to_int(msg) == pow(number, key.e, key.n):
		return flag
	else:
		return 'wrong signature'

def loop():
	while True:
		print('Choose an option:\n1:[text to sign]\n2:[number - signature to check]\n')
		sys.stdout.flush()
		cmd = sys.stdin.buffer.readline().strip()
		if cmd[:2] == b'1:':
			print(sign(cmd[2:]))
		elif cmd[:2] == b'2:':
			print(verify(int(cmd[2:].decode())))
		else:
			print('Wrong input format')

if __name__ == '__main__':
	try:
		loop()
	except Exception as err:
		print(repr(err))
