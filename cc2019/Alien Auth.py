#!/usr/bin/env python3
from functools import reduce
import requests
import sys

def gcd(a,b):
    return gcd(b, a % b) if b else a

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, x, y = egcd(b % a, a)
        return (g, y - (b // a) * x, x)

def modinv(b, n):
    g, x, _ = egcd(b, n)
    if g == 1:
        return x % n

def crack_unknown_increment(states, modulus, multiplier):
    increment = (states[1] - states[0]*multiplier) % modulus
    return modulus, multiplier, increment

def crack_unknown_multiplier(states, modulus):
    multiplier = (states[2] - states[1]) * modinv(states[1] - states[0], modulus) % modulus
    return crack_unknown_increment(states, modulus, multiplier)

def crack_unknown_modulus(states):
    diffs = [s1 - s0 for s0, s1 in zip(states, states[1:])]
    zeroes = [t2*t0 - t1*t1 for t0, t1, t2 in zip(diffs, diffs[1:], diffs[2:])]
    modulus = abs(reduce(gcd, zeroes))
    return crack_unknown_multiplier(states, modulus)

print('calculating initial rng state')
while True:
    session = requests.Session()
    values = []
    for i in range(25):
        r = session.get('http://172.16.4.144:7001/?p=0').text
        r = r[r.find('we want ') + 8\
            : r.find(' ', r.find('we want ') + 9)]
        values.append(int(r))
    try:
        m, a, b = crack_unknown_modulus(values)
        print('rng state found')
        break
    except:
        pass

print('getting flag')
s0 = values[-1]
while True:
    s1 = (a * s0 + b) % m
    s0 = s1
    r = session.get('http://172.16.4.144:7001/?p=' + str(s1)).text
    if 'SPRITZ' in r:
        print(r[r.find('<pre>') + 5
            : r.find('</pre>', r.find('<pre>') + 1) - 4])
        sys.exit(0)
