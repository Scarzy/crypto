#!/usr/bin/env python
import re

f = open("result20-26-58.bin","r")
read = f.read()

text = {}

r = list()
c = list()

for i in read:
	r.append(i)
	c.append(i)

for i in range(len(r)):
	if r[i] in text:
		text[r[i]] += 1
	else:
		text[r[i]] = 1
print text

def repl(char,dechar):
	for i in range(len(r)):
		if r[i] == char.upper():
			c[i] = dechar.upper()
		elif r[i] == char.lower():
			c[i] = dechar.lower()

#alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
#for j in alphabet:
#	for k in alphabet:
#		repl(j,k)
#		print ''.join(c)
for i in range(len(r)):
	repl('A','N')
	repl('B','O')
	repl('C','P')
#	repl('D','Q')
	repl('E','R')
	repl('F','S')
	repl('G','T')
	repl('H','U')
	repl('I','V')
	repl('J','W')
#	repl('K','X')
	repl('L','Y')
#	repl('M','Z')
	repl('N','A')
	repl('O','B')
	repl('P','C')
	repl('Q','D')
	repl('R','E')
	repl('S','F')
	repl('T','G')
	repl('U','H')
	repl('V','I')
#	repl('W','J')
	repl('X','K')
	repl('Y','L')
	repl('Z','M')
	pass

print ''.join(r)
print ''.join(c)
