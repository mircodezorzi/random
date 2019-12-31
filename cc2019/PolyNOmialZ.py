#!/usr/bin/env python3
from pwn import *
import os

if os.getenv('REMOTE'):
    p = remote('172.16.4.144', 7004)
else:
    p = process('./challenges/polynomialz.py')

p.sendline('256')
num = int(p.readline().replace('\n', ''))
out = []

while num > 1:
    out.append(chr(num % 256))
    num //= 256

p.sendline(bytes(bytearray(out)).encode('hex'))
print(p.readline().replace('\n', ''))
p.close()
