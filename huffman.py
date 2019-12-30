import codecs
import pickle
import math
import sys

def analyze(text):
    symbols = {}
    chars = 0

    for char in text:
        try:
            symbols[char] += 1
        except:
            symbols[char] = 1
        chars += 1

    for char in symbols:
        symbols[char] /= chars

    return list(symbols.keys()), list(symbols.values())

def parallel_sort(key, val):
    for _ in range(len(key) - 1):
        for i in range(len(key) - 1):
            if val[i] < val[i + 1]:
                key[i], key[i + 1] = key[i + 1], key[i]
                val[i], val[i + 1] = val[i + 1], val[i]
    return key, val

def train(s, p):
    codec = {}

    for c in s:
        codec[c] = ''

    while len(s) > 1:
        parallel_sort(s, p)

        last, slast  = s.pop(), s.pop()

        for c in last:
            codec[c] += '0'
        for c in slast:
            codec[c] += '1'

        last_s = last + slast
        last_p = p.pop() + p.pop()

        s.append(last_s)
        p.append(last_p)

    for c in codec:
        codec[c] = codec[c][::-1]

    return codec

def encode(filepath):
    with codecs.open(filepath, 'r', encoding='utf-8') as f:
        data = f.read()
    s, p = analyze(data)
    _s, _p = s.copy(), p.copy()

    codec = train(s, p)

    avg_len = sum(len(codec[c]) * _p[_s.index(c)] for c in codec)
    entropy = sum(n * math.log(1 / n, 2) for n in _p)

    print('entropy: {:0.2f}; average length: {:0.2f}; efficency: {:0.2f}'
        .format(
            entropy,
            avg_len,
            entropy / avg_len),
        end = '')

    print('initial size: {}\n'
        .format(
            len(data)),
        end = '')

    bytes = ''.join(codec[c] for c in data)
    padding = 8 - len(bytes) % 8
    bytes += '0' * padding
    data = ''.join(
        chr(int(bytes[i : i + 8], 2))\
        for i in range(0, len(bytes), 8))

    print('compressed size: {} (+ ~1500b)'.format(len(data)))

    with open(filepath + '.hz', 'wb') as f:
        pickle.dump([codec, padding, data.encode('charmap')], f)

    print('succesfully written {}'.format(filepath + '.hz'))


def decode(filepath):
    with open(filepath, 'rb') as f:
        codec, padding, data = pickle.load(f)

    data = data.decode('charmap')
    print('decoding {}...'.format(filepath))
    bytes = ''.join('{:08b}'.format(ord(c)) for c in data)
    bytes = bytes[:-padding]

    result = ''
    offset = 0
    reverse_codec = dict(zip(codec.values(), codec.keys()))
    while offset < len(bytes):
        for i in range(offset, len(bytes) + 1):
            if bytes[offset:i] in codec.values():
                result += reverse_codec[bytes[offset:i]]
                offset = i

    print('succesfully decoded {}'.format(filepath))

    with codecs.open(filepath[:-3], 'w', encoding='utf-8') as f:
        f.write(result)

if len(sys.argv) == 3:
    mode     = sys.argv[1]
    filepath = sys.argv[2]

    if mode == 'e':
        encode(filepath)
    elif mode == 'd':
        decode(filepath)

else:
    mode     = input('[e]ncode/[d]ecode: ')
    filepath = input('filepath: ')

    if mode == 'e':
        encode(filepath)
    elif mode == 'd':
        decode(filepath)
