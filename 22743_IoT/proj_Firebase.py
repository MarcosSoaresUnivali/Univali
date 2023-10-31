import requests
import json

link = "https://esp-firebase-iot-d644f-default-rtdb.firebaseio.com/"

# new request (POST) new record
record = {'cliente': 'musk', 'preco': 150, 'produto': 'keyboard'}
request = requests.post(f'{link}/Vendas/.json', data=record)
print(request)
print(request.text)