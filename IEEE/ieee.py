from os import system

def decToIEEE(dec):

    toHex = [
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'A', 'B',
        'C', 'D', 'E', 'F'
        ]

    bini, binf, bine = '', '', ''
    itbp, ftbp, etbp = '', '', ''
    sign, mantiss, exponent = '', '', ''

    ieee, ieeehex, counter = '', '', 0

    i = abs(int(dec[:dec.find('.')]))
    f = float('0.' + str(dec[dec.find('.') + 1:]))

    s = int(float(dec) < 0)
    sign = str(s)

    while i >= 1:
        itbp += '{}&{}\\\\\n'.format(int(i) , str(int(i % 2)))
        bini += str(int(i % 2))
        i /= 2
    bini = bini[::-1]

    while f != 0 and counter < 24 - len(bini):
        ftbp += '{} * 2 = {}&{}\\\\\n'.format(round(f, 5), round(f, 5) * 2, int(f * 2 >= 1))
        counter += 1
        f *= 2
        binf += str(int(f >= 1))
        f -= int(f >= 1)

    m = bini + binf
    mantiss = '{}.{}'.format(bini, binf)

    fo = m.find('1')
    e = len(bini) - fo + 126
    exponent = str(e)

    while e >= 1:
        etbp += '{}&{}\\\\\n'.format(int(e) , str(int(e % 2)))
        bine += str(int(e % 2))
        e /= 2
    if len(bine) == 7:
        bine += '0'
    bine = bine[::-1]

    ieee = str(s) + bine + m[1:]
    ieee += '0' * (32 - len(ieee))

    ieeeh = ''
    for c in range(0, len(ieee) - 1, 4):
        r = 0
        for i in range(4):
            r += int(ieee[c + i : c + i + 1] == '1') * (2 ** (3 - i))
        ieeehex += toHex[r]

    template = '\\section{DECIMAL}\\begin{multicols}{3}\\begin{center}\n\\begin{tabular}{r | l}\nTABLE1\\end{tabular}\n\n\\begin{tabular}{r | l}\nTABLE2\\end{tabular}\n\n\\begin{tabular}{r | l}\nTABLE3\\end{tabular}\n\\end{center}\\end{multicols}\ns = SIGN\n\nm = MANTISS$_2$\n\ne = EXPONENT1 $\\rightarrow$ EXPONENT2 $\\rightarrow$ EXPONENT_BIN$_2$\n\nDECIMAL$_{10}$ = BINARY$_2$ = HEX$_{16}$\\pagebreak'

    with open('ieee.tex', 'r+') as f:
        tex = f.read()
        tex = tex.replace('%APPEND', '{}\n\n%APPEND'.format(template))
        tex = tex.replace('TABLE1', itbp)
        tex = tex.replace('TABLE2', ftbp)
        tex = tex.replace('TABLE3', etbp)
        tex = tex.replace('SIGN', sign)
        tex = tex.replace('MANTISS', mantiss)
        tex = tex.replace('EXPONENT1', str(int(exponent) - 127))
        tex = tex.replace('EXPONENT2', exponent)
        tex = tex.replace('EXPONENT_BIN', bine)
        tex = tex.replace('DECIMAL', dec)
        tex = tex.replace('BINARY', ieee)
        tex = tex.replace('HEX', ieeehex)
        f.seek(0)
        f.write(tex)

def IEEEtoDec(hex):

    toDec = {
        'A': 10, 'B': 11,
        'C': 12, 'D': 13,
        'E': 14, 'F': 15
        }

    tohex_process = ''
    mantissa_process = ''
    exponent_process = ''

    bin = ''
    e, m, dec = 0, 0, 0

    hex += '0' * (8 - len(hex))
    for c in hex:
        tohex_process += c
        if not c.isnumeric():
            c = toDec[c]
        else:
            c = int(c)

        binc = ''
        for i in range(4):
            binc += str(int(c % 2))
            c /= 2
        bin += binc[::-1]

        tohex_process += ' = {} \\\\\n'.format(binc[::-1])

    sign = bin[:1]
    exponent = bin[1:9]
    mantissa = bin[9:]

    for i in range(8):
        e += int(exponent[i]) * (2 ** (len(exponent) - i - 1))
    e -= 127

    mantisse = '1' + mantissa
    if e < 0:
        mantissa = ('0' * e) + mantissa

    bini = mantissa[:e+1]
    binf = mantissa[e+1:]

    for i in range(len(bini)):
        dec += int(bini[i]) * (2 ** (len(bini) - i - 1))
        mantissa_process += '$2^{{{}}}$ & + \\\\\n'.format(len(bini) - i - 1) * int(bini[i])
    for i in range(len(binf)):
        dec += int(binf[i]) * (2 ** (-i - 1))
        mantissa_process += '$2^{{{}}}$ & + \\\\\n'.format(-i - 1) * int(binf[i])
    mantissa_process = mantissa_process[:-1] + '& = ' + str(dec) + '\\\\\n '

    template = '\\section{HEX}\\begin{multicols}{2}\\begin{center}\\begin{tabular}{c}\nTOHEX_PROCESS\\end{tabular}\n\n\\begin{tabular}{r | l}\nMANTISSA_PROCESS\\end{tabular}\n\n\\begin{tabular}{r | l}\nEXPONENT_PROCESS\\end{tabular}\n\\end{center}\\end{multicols}\n\ns = SIGN\n\nm = MANTISSA $_2$\n\ne = EXPONENT_BIN$_2$ $\\rightarrow$ EXPONENT_127$_10$ $\\rightarrow$ EXPONENT$_2$\n\nHEX$_{16}$ = BINARY$_2$ = DECIMAL$_{10}$\\pagebreak'

    with open('ieee.tex', 'r+') as f:
        tex = f.read()
        tex = tex.replace('%APPEND', '{}\n\n%APPEND'.format(template))
        tex = tex.replace('SIGN', sign)
        tex = tex.replace('TOHEX_PROCESS', tohex_process)
        tex = tex.replace('MANTISSA_PROCESS', mantissa_process)
        tex = tex.replace('EXPONENT_PROCESS', exponent_process)
        tex = tex.replace('EXPONENT_BIN', exponent)
        tex = tex.replace('EXPONENT_127', str(e + 127))
        tex = tex.replace('EXPONENT', str(e))
        tex = tex.replace('MANTISSA', '{}.{}'.format(mantissa[:e+1], mantissa[e+1:]))
        tex = tex.replace('HEX', hex)
        tex = tex.replace('BINARY', bin)
        tex = tex.replace('DECIMAL', str(dec))
        f.seek(0)
        f.write(tex)

system('clear')
while True:

    try:
        type = input('type: ')
    except:
        break

    if type == 'clear':
        print('resetting latex file...')
        system('cp template.tex ieee.tex')
        type = input('type: ')

    while True:
        try:
            if type == 'tohex':
                decToIEEE(input('DEC: '))
            elif type == 'todec':
                IEEEtoDec(input('HEX: '))
        except:
            break

system('pdflatex ieee.tex')
system('mupdf ieee.pdf')
system('clear')
