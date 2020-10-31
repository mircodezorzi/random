import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import requests
import sys
import threading
import time


sample = 10
timeout = 10
url = 'http://192.168.4.1'

c = threading.Condition()
connected = False
data = []
fig = plt.figure()
running = True

def init():
    print('connecting to local web server on', url)
    t.start()
    p.start()
    if not running: sys.exit(0)
    while not connected: pass
    print('collecting initial data, please wait')
    while len(data) < sample: pass

def read():
    global running
    global connected
    while running:
        try:
            r = requests.get(url, timeout = timeout)
            connected = True
            data.append(r.content.decode().split(' '))
        except requests.ConnectTimeout:
            print('connection timed out,\
                make sure you\'re connected to the arduino web server')
            running = False
        except requests.exceptions.MissingSchema:
            print('invalid url schema')
            running = False
        time.sleep(0.5)

def generate_image():
    global running
    while running:
        fig.savefig('plot.png')
        time.sleep(0.5)

tmp_ax = fig.add_subplot(2, 2, 1)
tmp_ax.set_ylim([20, 50])
plt.yticks([20, 50])
plt.xlabel('')
hum_ax = fig.add_subplot(2, 2, 2)
hum_ax.set_ylim([0, 100])
plt.yticks([0, 100])
plt.xlabel('')
co2_ax = fig.add_subplot(2, 1, 2)
co2_ax.set_ylim([0, 1])
plt.yticks([0, 1])
plt.xlabel('')

tmp_x = []
tmp_y = []
def plot_tmp(_):
    new = data[-sample:]
    ys = [int(new[i][1]) for i in range(len(new))]
    xs = int(new[-1][0])
    tmp_x.append(xs)
    tmp_y.append(sum(ys) / len(ys))
    if len(tmp_x) > 100:
        tmp_x.pop(0)
        tmp_y.pop(0)
    with open('tmps', 'w') as f:
        f.write('{} {}'.format(x, y)
            for x, y in zip(tmp_x, tmp_y))
    pl = np.polyfit(tmp_x, tmp_y, 2)
    tmp_ax.clear()
    tmp_ax.plot(tmp_x, tmp_y, 'b-')
    tmp_ax.plot(tmp_x, np.polyval(pl, tmp_x), 'b:')

hum_x = []
hum_y = []
def plot_hum(_):
    new = data[-sample:]
    ys = [int(new[i][2]) for i in range(len(new))]
    xs = int(new[-1][0])
    hum_x.append(xs)
    hum_y.append(sum(ys) / len(ys))
    if len(hum_x) > 100:
        hum_x.pop(0)
        hum_y.pop(0)
    with open('hums', 'w') as f:
        f.write('{} {}'.format(x, y)
            for x, y in zip(hum_x, hum_y))
    pl = np.polyfit(hum_x, hum_y, 2)
    hum_ax.clear()
    hum_ax.plot(hum_x, hum_y, 'g-')
    hum_ax.plot(hum_x, np.polyval(pl, hum_x), 'g:')

if __name__ == '__main__':
    try:
        t = threading.Thread(target=read, args=())
        p = threading.Thread(target=generate_image, args=())
        init()
        tmp_anim = animation.FuncAnimation(fig, plot_tmp, interval = 5000)
        hum_anim = animation.FuncAnimation(fig, plot_hum, interval = 5000)
        plt.show()
    except KeyboardInterrupt:
        print('exiting')
        running = False
        sys.exit(0)
