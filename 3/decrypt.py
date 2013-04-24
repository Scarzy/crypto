#!/usr/bin/env python
import re

f = open("text","r")
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
#print text

def repl(char,dechar):
	for i in range(len(r)):
		if r[i] == char.upper():
			c[i] = dechar.upper()
		elif r[i] == char.lower():
			c[i] = dechar.lower()

for i in range(len(r)):
#	repl('R','I')
#	repl('Z','A')
#	repl('V','E')
	repl('A','Z')
	repl('B','Y')
	repl('C','X')
	repl('D','W')
	repl('E','V')
	repl('F','U')
	repl('G','T')
	repl('H','S')
	repl('I','R')
	repl('J','Q')
	repl('K','P')
	repl('L','O')
	repl('M','N')
	repl('N','M')
	repl('O','L')
	repl('P','K')
	repl('Q','J')
	repl('R','I')
	repl('S','H')
	repl('T','G')
	repl('U','F')
	repl('V','E')
	repl('W','D')
	repl('X','C')
	repl('Y','B')
	repl('Z','A')

print ''.join(r)
print ''.join(c)
