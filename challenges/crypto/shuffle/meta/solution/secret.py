#!/usr/bin/python3
import random
import math

flag = 'ENO{maybe_y0u_shou1d_use_an0ther_group_for_DH}'

def cycles(perm):
	#https://codereview.stackexchange.com/questions/201725/disjoint-cycles-of-a-permutation
	mappings = { a: b for a, b in enumerate(perm) if a != b }
	cycles = []
	for a in perm:
		b = mappings.pop(a, None)
		if b is None:
			continue # `a` has already been handled
		cycle = [a]
		while a != b:
			cycle.append(b)
			b = mappings.pop(b)
		cycles.append(cycle)
	return cycles

def lcm(l):
	if l == []: return 1
	intermediate = lcm(l[1:])
	return l[0] * intermediate // math.gcd(l[0], intermediate)

def order(p):
	#keep secret
	return lcm([len(c) for c in cycles(p)])

def generate_base(n):
	for _ in range(n):
		p = list(range(n))
		random.shuffle(p)
		if order(p) > 1<<80: return p
	raise Exception('No generator found')
