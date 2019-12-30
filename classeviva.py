import classeviva
import json
import os
import random
import requests
import subprocess
import sys
import threading

user = os.getenv('USER')
folder = '/home/{}/.cache/classeviva/'.format(user)
cache_folder = '/home/{}/.cache/classeviva/'.format(user)
pdf_viewer = 'zathura'
timeout = 25

if not os.path.exists(folder):
    os.makedirs(folder, exist_ok=True)

username = ''
password = ''
chat_id  = ''
token    = ''

noticeboard_read = 'https://web.spaggiari.eu/rest/v1/students/{}/noticeboard/read/{}/{}/101'
noticeboard_attach = 'https://web.spaggiari.eu/rest/v1/students/{}/noticeboard/attach/{}/{}/101'

def send_document(chat_id, path, caption, token = token):
    os.system('curl\
        -F chat_id="{}"\
        -F document=@"{}"\
        -F caption="{}"\
        https://api.telegram.org/bot{}/sendDocument'.format(
            chat_id, path, caption, token
        )
    )

def send_image(chat_id, path, caption, token = token):
    os.system('curl\
        -F chat_id="{}"\
        -F photo=@"{}"\
        -F caption="{}"\
        https://api.telegram.org/bot{}/sendPhoto'.format(
            chat_id, path, caption, token
        )
    )

def create_image(path):
    os.system(
        'pdftoppm -f 1 -l 1\
            -scale-to-x 1920\
            -scale-to-y -1\
            -singlefile\
            -jpeg -tiffcompression jpeg --\
            "{}" {}tmp'.format(
                path, folder
            )
    )

def sanitize_str(str):
    return str.replace('_', ' ')\
              .replace('-', ' ')

def send_notification(description, body, path, image = folder + 'tmp.jpg', time = 5000):
    def deamon():
        pipe = ['dunstify', '--action=replyAction,reply',
                description, body,
                '-i', image,
                '-t', str(time)]

        if subprocess.Popen(pipe, stdout=subprocess.PIPE).communicate()[0] == b'2\n':
            os.system('{} "{}"'.format(pdf_viewer, path))

    t = threading.Thread(target=deamon, args = ())
    t.start()

s = classeviva.Session()

if username and password:
    s.login(username, password)
else:
    for i in range(3):
        try:
            username = input('username: ')
            os.system('stty -echo')
            password = input('password: ')
            os.system('stty echo')
            s.login(username, password)
            break
        except KeyboardInterrupt:
            os.system('stty echo')
            sys.exit(0)
        except:
            print('login failed')
    if not s.logged_in:
        print('failed after 3 tries')
        sys.exit(0)
    else:
        print()

def update_noticeboard():

    noticeboard = s._noticeboard()['items']

    for notice in noticeboard:

        filepath = folder + notice['cntTitle'] + '.pdf'

        if not os.path.exists(filepath):

            print('downloading', notice['cntTitle'], '...')

            url = noticeboard_read.format(s.id, notice['evtCode'], notice['pubId'])

            r = requests.post(
                url=url,
                headers={
                    "User-Agent": "zorro/1.0",
                    "Z-Dev-Apikey": "+zorro+",
                    "Z-Auth-Token": s.token,
                    "Content-Type": "application/pdf",
                },
                timeout = timeout
            )

            url = noticeboard_attach.format(s.id, notice['evtCode'], notice['pubId'])

            r = requests.get(
                url=url,
                headers={
                    "User-Agent": "zorro/1.0",
                    "Z-Dev-Apikey": "+zorro+",
                    "Z-Auth-Token": s.token,
                    "Content-Type": "application/pdf",
                },
                timeout = timeout
            )

            if not r.headers['Content-Type'] == 'application/json; charset=utf-8':
                with open(filepath, 'wb') as f:
                    f.write(r.content)
                    create_image(filepath)
                    send_notification('Nuova circolare!', sanitize_str(notice['cntTitle']), filepath)
                    #send_document(chat_id, filepath, sanitize_str(notice['cntTitle']))
        else:
            print('already downloaded, skipping')

def generate_number(grade, filepath):
    color = 'red' if grade < 6 else 'green'
    os.system('\
        convert\
        -size 64x64\
        xc:{}\
        -pointsize 30\
        -fill white\
        -gravity center\
        -draw "text 1,3 \'{}\'"\
        {}'.format(
            color,
            grade,
            filepath
        )
    )

update_noticeboard()
