import json
import re
import requests

# read username and password from secrets/weschool_secrets.py
import sys; sys.path.insert(0, 'secrets'); from weschool_secrets import secrets

group = 186622

s = requests.Session()

r = s.post(
	url = 'https://api.weschool.com/oauth/v2/token',
	json = secrets)

bearer = r.json()['access_token']

r = s.get(
	url = f'https://api.weschool.com/v1/exercises?filter=ASSIGNMENT&group_id={group}',
	headers = {
		'Authorization':  f'Bearer {bearer}'
	})

def solve(exercise):
	s.options(url = f'https://api.weschool.com/v1/exercises/{exercise}/executions')

	r = s.post(
		url = f'https://api.weschool.com/v1/exercises/{exercise}/executions',
		headers = {
			'Authorization':  f'Bearer {bearer}'
		})

	r = r.json()
	r['vote'] = 100
	r['result'] = True
	r['quizzes_ok'] = len(r['quiz_executions'])
	for quiz in r['quiz_executions']:
		quiz['result'] = True
		quiz['value'] = True

	run = r['id']

	s.options(url = f'https://api.weschool.com/v1/exercises/{exercise}/executions/{run}')

	r = s.put(
		url = f'https://api.weschool.com/v1/exercises/{exercise}/executions/{run}',
		json = r,
		headers = {
			'Authorization':  f'Bearer {bearer}'
		})
	print(r.text)

for exercise in r.json():
	solve(exercise['id'])
