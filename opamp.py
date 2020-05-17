#!/usr/bin/env python3

import hashlib
import os

diagram = {
  'kind': '+',
  'operators': [
    {
      'kind': '+',
      'multiplier': 4,
      'operators': [
        {
          'kind': 'source',
          'label': 'V_1',
          'multiplier': 2
        },
        {
          'kind': 'source',
          'label': 'V_2',
        },
        {
          'kind': 'source',
          'label': 'V_3',
          'multiplier': 3
        }
      ]
    },
    {
      'kind': '+',
      'operators': [
        {
          'kind': '+',
          'operators': [
            {
              'kind': 'source',
              'label': 'V_4',
              'multiplier': 4
            },
            {
              'kind': 'source',
              'label': 'V_5',
            }
          ]
        },
        {
          'kind': '+',
          'operators': [
            {
              'kind': 'source',
              'label': 'V_6',
            },
            {
              'kind': 'source',
              'label': 'V_7',
              'multiplier': 4
            }
          ]
        },
        {
          'kind': '+',
          'operators': [
            {
              'kind': 'source',
              'label': 'V_8',
            },
            {
              'kind': 'source',
              'label': 'V_9',
            }
          ]
        }
      ]
    },
  ]
}

f = open('.cache/output.tex', 'w')
f.write(r'\documentclass{standalone}\usepackage[utf8]{inputenc}\usepackage[english]{babel}\usepackage{circuitikz}\begin{document}\begin{circuitikz}\draw ')

def depth(ast):
  if type(ast) == list or (type(ast) == dict and ast['kind'] != 'source'):
    return 1 + max([depth(child) for child in ast])
  return 1

max_depth = depth(diagram)

def traverse(ast, depth = 0, x = 0, y = 0, parent = ''):
  coords = (x, y)
  name   = 'opamp' + hashlib.md5('{}'.format(coords).encode('utf-8')).hexdigest()[:5]

  if ast['kind'] == '+':
    f.write('({}, {}) node[op amp] ({}) {{}} ({}.-) to[short, *-] ++ (0, 1) coordinate (foo) to[R] (foo -| {}.out) to ({}.out)\n'
            .format(coords[0], coords[1], name, name, name, name, name))
    f.write('({}.+) node[ground] {{}} ++ (0, -1)'
            .format(name))
    if parent != '':
      f.write('({}.out) to[R,l=${}R$] ({}) '.format(name, ast.get('multiplier', ''), 'B' + parent))
    f.write('({}.-) to[short] ++ (-1, 0) coordinate ({})\n'
            .format(name, 'A' + name))

    y0 = y + 0.5
    for idx, operator in enumerate(ast['operators']):
      y -= 3.5
      if operator['kind'] == 'source': # generate Vin
        f.write('({}) to[short] ++ (0, {}) coordinate ({})\n'
                .format('A' + name, -idx, 'B' + name))
        f.write('({}) to[short] ++ (0, {}) to[short] ++ (-{}, 0) to[R,l=${}R$] ++ (-2.5, 0) to[short,-o,l=${}$] ++ (-0.5, 0)\n'
                .format('A' + name, -idx, max_depth * 6 + x, operator.get('multiplier', ''), operator['label']))
      else:
        f.write('({}) to[short] ++ (0, {}) coordinate ({})\n'
                .format('A' + name, y - y0, 'B' + name))
        traverse(operator, depth + 1, x - 6, y, name)

traverse(diagram)

f.write(';\end{circuitikz}\end{document}')
f.close()

os.system('cd .cache; pdflatex output.tex')
