#!/usr/bin/env python3

import fileinput

words = set()
revs = set()
for line in fileinput.FileInput():
	w = line.rstrip()
	rev = w[::-1]
	if w != rev: # not palindrom
		words.add(w)
		revs.add(rev)

result = list(words & revs)
		
print(sorted(result))