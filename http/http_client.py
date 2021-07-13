import requests

payload = {'firstName': 'John', 'lastName': 'Smith'}
r = requests.post("https://httpbin.org/post", json=payload)
print(r.json()['data'])