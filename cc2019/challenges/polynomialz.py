#!/usr/bin/env python2
# -*- coding: utf-8 -*-
from os import urandom


def f(a, b):
    if len(b) == 1:
        return b[0]

    return b[0] + a * f(a, b[1:])


#with open('flag.txt', 'rb') as fi:
#    flag = fi.read().strip()
flag = 'this is your flag'

rbytes = bytearray('~' + urandom(48) + '~')  # ~ = best character in the world

a = int(raw_input().strip())
print(f(a, rbytes))

a = raw_input().strip()

if a == bytes(rbytes).encode('hex'):
    print flag
else:
    print 'NO'
