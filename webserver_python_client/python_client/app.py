#!/usr/bin/env python
import sys, os, json, time
sys.path.append(os.path.join(os.path.dirname(__file__),"httplib2/python2/"))

import httplib2
http = httplib2.Http()

if 0:
  url = 'http://192.168.0.103/toggle'   
  response, content = http.request(url, 'GET')
  print response
  print content

if 0:
  url_json = 'http://192.168.0.103/jblink'   
  data = {'times': '10', 'pause': '500'}
  headers={'Content-Type': 'application/json; charset=UTF-8'}
  response, content = http.request(url_json, 'POST', headers=headers, body=json.dumps(data))
  print response
  print content

if 1:
  import urllib
  url_query = 'http://192.168.0.103/qblink'
  data = {'times': '10', 'pause': '500'}
  headers={'Content-Type': 'application/json; charset=UTF-8'}
  response, content = http.request(url_query, 'POST', headers=headers, body=urllib.encode(data))
  print response
  print content
