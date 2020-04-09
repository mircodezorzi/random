#!/usr/bin/env python

import cv2
import numpy as np
from math import sqrt

import os
import sys

image = cv2.imread(sys.argv[1])

# kernel = np.ones((5, 5), np.float32) / 25
# image  = cv2.filter2D(image, -1, kernel)

gray  = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
edged = cv2.Canny(gray, 25, 225)

contours, _ = cv2.findContours(
  edged,
  cv2.RETR_EXTERNAL,
  cv2.CHAIN_APPROX_NONE)


f = open(sys.argv[1] + '.sh', 'w')
f.write('xdotool mousemove {} {}\nxdotool click 1\nxdotool mousedown 1\n'.format(contours[0][0][0][0] + 225, contours[0][0][0][1] + 225))
cv2.drawContours(image, contours, -1, (0, 255, 0), 1)

cv2.imshow('Canny Edges After Contouring', image)

lx = contours[0][0][0][0] + 225
ly = contours[0][0][0][1] + 225

for c in contours:
    for i in c[::3]:
        x = i[0][0] + 225
        y = i[0][1] + 225
        if sqrt(abs(x - lx) ** 2 + abs(y - ly)) > 50:
            f.write('xdotool mouseup 1\n')
        f.write('xdotool mousemove {} {}\n'.format(x, y))
        if sqrt(abs(x - lx) ** 2 + abs(y - ly)) > 50:
            f.write('xdotool mousedown 1\n')
        lx = x
        ly = y

f.write('xdotool mouseup 1')
f.close()
os.system('')

cv2.waitKey(0)
