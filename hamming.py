import functools
import itertools
import random
import math
import copy
import sys

def bool_to_bin(x):
    return '1' if x else '0'

# convert list of chars to list of 0s and 1s
def char_to_bool(lst):
    return [True if ord(str(i)) - 0x30 else False for i in lst]

# convert a character to list of 0s and 1s
def char_to_byte(lst):
    return [char_to_bool(i) for i in ['{:08b}'.format(ord(i)) for i in lst]]

def bool_to_str(lst):
    return ''.join(['1' if i else '0' for i in lst])

def xor_all(lst):
    return functools.reduce(lambda x, y: x ^ y, lst)

def parity(data, type = 'even'):
    return xor_all(data) ^ 0 if type == 'even' else 1

def print_red(msg, end):
    print('\033[91m{}\033[0m'.format(msg), end = end)

def print_diff(d1, d2):
    if sys.platform == 'linux':
        for i in range(len(d1)):
            if not (d1[i] == d2[i]):
                print_red(bool_to_bin(d2[i]), end = '')
            else:
                print(bool_to_bin(d2[i]), end = '')
    else:
        for i in range(len(d1)):
            print(bool_to_bin(d1[i]), end = '')
        print(' ', end = '')
        for i in range(len(d2)):
            print(bool_to_bin(d2[i]), end = '')
    print() # newline

# return list of all powers of two of x
def get_powers_of_two(x):
    result = []
    i = 1 << math.floor(math.log(x, 2))
    while x > 0:
        if x >= i:
            result.append(i)
            x -= i
        i >>= 1
    return result

def is_power_of_two(x):
    while x > 1:
        if x % 2:
            return False
        x >>= 1
    return True

# given the list
# [ [11, [8, 4]],    [10, [8, 2, 1]],
#   [9,  [8, 2]],    [8,  [8, 1]],
#   [6,  [4, 2, 1]], [5,  [4, 2]],
#   [4,  [4, 1]],    [2,  [2, 1]] ]
# find_bits(4, list) returns [11, 6, 5, 4, 2]
def find_bits(x, control_bits):
    result = []
    for cb in control_bits:
        if x in cb[1]:
            result.append(cb[0])
    return result

# I know this goes against my principle of not hard coding anything and
# trying to make everything scalable, and this is the reason it took me
# almost 200 lines to write this simple program  but I'm  honestly too
# lazy to calculate the formula for this so I'll have  to hard code this in
hamming_code_sizes = {
      1: 3,     4: 7,
      8: 12,   11: 15,
     26: 31,   57: 63,
    120: 127, 247: 255
}

def encode(data):
    # this assert and the next one in the code aren't actually necessary
    # as the sizes of the parameters parsed don't directly rely on user
    # input so there's no way for the data size not to match
    assert len(data) in hamming_code_sizes, 'data size not supported'
    # automatically generate array to store hamming code in
    ham = [0 for _ in range(hamming_code_sizes[len(data)])]
    parity_scheme = []

    # copy data into hamming code and calculate parity_scheme
    # creates list parsed to find_bits with a tuple with:
    #     x and the powers of two of x
    counter = 0
    for i in range(len(ham)):
        if not is_power_of_two(len(ham) - i):
            ham[i] = data[counter]
            parity_scheme.append(
                (len(ham) - i - 1,
                get_powers_of_two(len(ham) - i)))
            counter += 1

    # for every position that's a power of two:
    # calculate the xor of all the bits in the parity scheme
    for i in range(0, math.floor(math.log(len(ham), 2)) + 1):
        # these three lines are actually one entire command
        ham[len(ham) - (0 if i == 0 else (1 << i) - 1) - 1] =\
            xor_all([ham[len(ham) - j - 1]
                for j in find_bits(1 << i, parity_scheme)])
    ham.insert(0, parity(ham))
    return ham

# given a list strips all elements of position len(lst) -  power of two
def strip_non_powers_of_two(lst):
    result = []
    for i in range(len(lst)):
        if not is_power_of_two(len(lst) - i):
            result.append(lst[i])
    return result

def check(ham):
    # this is the other non-necessary assert
    # the - 1 is to account for the parity bit
    assert len(ham) - 1 in hamming_code_sizes.values(),\
        'data size not supported'

    error_pos = 0
    print(bool_to_str(ham), end = '')
    checked_parity = not parity(ham)
    parity_bit = ham.pop(0)

    checked = encode(strip_non_powers_of_two(ham))
    # xor between parity bits calculated and parity bits parsed
    for i in range(0, math.floor(math.log(len(ham), 2)) + 1):
        error_pos += int(ham[len(ham) - (0 if i == 0 else (1 << i) - 1) - 1]\
            ^ checked[len(checked) - (0 if i == 0 else (1 << i) - 1) - 1])\
            * 1 << i

    if checked_parity and error_pos:
        print(' error detected but not able to correct')
    elif error_pos or (parity(ham) != parity_bit):
        print(' attempting to correct error at position {}'
            .format(len(ham) - error_pos))
        # if there are more than three errors in the data the program will
        # try to correct them even when it shouldn't. In these cases there's
        # the chance that the calculated index will be negative
        try:
            ham[len(ham) - error_pos] = not ham[len(ham) - error_pos]
        except:
            pass
    else:
        print()

    ham.insert(0, parity_bit)

# negate random elements of 2 dimensional list of bool values
def corrupt(ham, density):
    for _ in range(sum(len(elem) for elem in ham) // density):
        i = random.randint(0, 1000)
        ham[i % len(ham)][i % len(ham[0])] =\
            not ham[i % len(ham)][i % len(ham[0])]

if __name__ == '__main__':

    hamming_code = []

    if len(sys.argv) > 1:
        data = sys.argv[1]
    else:
        while True:
            data = input('>>> ')
            if data: break

    print('> generating hamming code...')
    for byte in char_to_byte(data):
        hamming_code.append(encode(byte))

    for ham in hamming_code:
        print(bool_to_str(ham))

    orig_hamming = copy.deepcopy(hamming_code)

    print('> applying error rate of 10%...')
    corrupt(hamming_code, 10)
    for o, h in zip(orig_hamming, hamming_code):
        print_diff(o, h)

    print('> checking for errors...')
    for ham in hamming_code:
        check(ham)

    print('> corrected data...')
    for o, h in zip(orig_hamming, hamming_code):
        print_diff(o, h)
