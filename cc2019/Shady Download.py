#!/usr/bin/env python2
import base64
import sys

def nsplit(it, n):
    return [it[j:j+n] for j in range(0, len(it), n)]

base='PikhLDcoHhdbCBQSBgsqAjsNXxocExkJPgJIGQAvERwMK1BfHQ4='
key = 'mysecretkeyshouldnothaveanyrepetitions'

base = base64.b64decode(base).encode('hex')

for i, j in zip(key, nsplit(base, 2)):
    sys.stdout.write(chr(ord(i) ^ int(j, 16)))
