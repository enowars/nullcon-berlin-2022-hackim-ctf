#!/usr/bin/env python3
from Crypto.PublicKey import RSA
from Crypto.Util import number
from Crypto.Util.number import bytes_to_long, long_to_bytes, inverse
from sympy.ntheory.continued_fraction import continued_fraction_iterator, continued_fraction_reduce
from gmpy2 import is_prime
from binascii import hexlify, unhexlify
import numpy as np
import math
import random
import sys
import sympy
import cypari
import base64
sys.setrecursionlimit(5000)
sympy.init_printing()

ggt = math.gcd

def factorint(number):
	res = cypari.pari('factor({})'.format(number))
	return {res[0][i] : res[1][i] for i in range(len(res[0]))}

def phi(n, factors = None):
	if factors is None:
		factors = sympy.ntheory.factorint(n)
	return prod([(k - 1) * k**(factors[k] - 1) for k in factors.keys()])

def chunks(l, n):
    """Yield successive n-sized chunks from l."""
    for i in range(0, len(l), n):
        yield l[i:i + n]

def write(name, message):
	if type(message) == int:
		message = str(message)
	writer = open(name,'w')
	writer.write(message)
	writer.close()

def random_string(length):
	return ''.join([chr(random.randint(32,255)) for _ in range(length)])

def fib(n):
	a,b = 1,0
	for _ in range(n):
		a,b = a+b, a
	return b

def EEA(a,b):
	if b == 0: return (a, 1, 0)
	d,s,t = EEA(b, a % b)
	return (d, t, s - (a//b) * t)

def prod(l):
	if len(l) == 0: return 1
	return l[0] * prod(l[1:])

def phi_fac(n, factors = None):
	if factors is None:
		factors = sympy.ntheory.factorint(n)
	return prod([(k - 1) * k**(factors[k] - 1) for k in factors.keys()])

def crt_pairs(pairs):
	if len(pairs) == 1: 
		a,n = pairs[0]
		return (a % n), n
	a0,n0 = pairs[0]
	a1,n1 = pairs[1]
	d,s,t = EEA(n0, n1)
	if d > 1: raise Exception('not coprime')
	x = a1*s*n0 + a0*t*n1
	return crt_pairs([(x, n0*n1)] + pairs[2:])

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

def bisect(f, low, up, rounding = 0):
	flow = f(low)
	fup = f(up)
	if flow == 0: return low
	if fup == 0: return up
	if flow * fup > 0: raise ValueError('bad interval')
	if flow < 0:
		return _bisect(f, low,up,rounding)
	else:
		return _bisect(lambda x: -f(x), low, up,rounding)

def _bisect(f, low, up, rounding):
	"""Find root by bisection. Require: f(low) < 0 < f(up)."""
	if up <= low + 1:
		if rounding == 1:
			return up
		elif rounding == -1:
			return low
		else:
			raise ValueError('no root or bad function')
	mid = (low + up) // 2
	midval = f(mid)
	if midval == 0: return mid
	if midval < 0: return _bisect(f, mid, up, rounding)
	if midval > 0: return _bisect(f, low, mid, rounding)

def cf_list(number, iterations):
	res = []
	cf = []
	gen = continued_fraction_iterator(number)
	for _ in range(iterations):
		cf.append(next(gen))
		frac = continued_fraction_reduce(cf)
		res.append(frac)
	return res

def complete_key(n,e, d = None, phi = None, p = None):
	if p is not None:
		q = n // p
		phi = (p-1) * (q-1)
		d = inverse(e, phi)
	elif phi is not None:
		a = n - phi
		disc = bisect(lambda x:((a+1)//2)**2 - n - x**2, 0, n)
		p = (a+1)//2 - disc
		q = n // p
		d = inverse(e, phi)
	elif d is not None:
		k = e * d - 1
		s = 0
		while k % 2 == 0:
			k //= 2
			s += 1
		for _ in range(10):
			a = random.randint(0, n - 1)
			p = ggt(a, n)
			if p > 1:
				return p, n // p
			base = pow(a, k, n)
			for i in range(s):
				p = ggt(base - 1, n)
				if p > 1 and p < n:
					return p, n // p
				base = pow(base, 2, n)
		raise Exception('No factor found')
	else:
		raise Exception('give at least one secret parameter')
	return {'n': n, 'e': e, 'd': d, 'phi': phi, 'p': p, 'q': q}

def erathosthenes(n):
	a = np.ones(n, dtype = np.bool)
	a[:2] = 0
	a[4::2] = 0
	i = 3
	while i**2 < n:
		if a[i]: 
			a[i**2::i] = 0
		i += 1
	return np.nonzero(a)[0]
