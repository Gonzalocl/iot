
import argparse
import requests
import time
import random

template_url = 'http://{}:1026/v1/updateContext'
template_payload = {
    'contextElements': [
        {
            'type': 'Laboratorio',
            'isPattern': 'false',
            'id': '',
            'attributes': [
                {
                    'name': 'temperature',
                    'type': 'float',
                    'value': 0
                },
                {
                    'name': 'humidity',
                    'type': 'float',
                    'value': 0
                }
            ]
        }
    ],
    'updateAction': ''
}

def ngsi_create(fiware_ip, fiware_lab, temperature, humidity):
    return ngsi_send_request(fiware_ip, fiware_lab, temperature, humidity, 'APPEND')

def ngsi_update(fiware_ip, fiware_lab, temperature, humidity):
    return ngsi_send_request(fiware_ip, fiware_lab, temperature, humidity, 'UPDATE')

def ngsi_send_request(fiware_ip, fiware_lab, temperature, humidity, updateAction):
    url = template_url.format(fiware_ip)
    template_payload['contextElements'][0]['id'] = 'Laboratorio{}'.format(fiware_lab)
    template_payload['contextElements'][0]['attributes'][0]['value'] = temperature
    template_payload['contextElements'][0]['attributes'][1]['value'] = humidity
    template_payload['updateAction'] = updateAction
    response = requests.post(url, json=template_payload).json()
    return 'contextResponses' in response \
        and len(response['contextResponses']) > 0 \
        and 'statusCode' in response['contextResponses'][0] \
        and 'code' in response['contextResponses'][0]['statusCode'] \
        and response['contextResponses'][0]['statusCode']['code'] == '200'

ip = ''
lab = ''

def random_temperature_humidity():
    return random.randrange(15, 25), random.randrange(75, 85)

def setup():
    temperature, humidity = random_temperature_humidity()
    if ngsi_create(ip, lab, temperature, humidity):
        print('Temperature: {} // Humidity: {}'.format(temperature, humidity))
    else:
        print('CREATE FAILED')

def loop():
    time.sleep(5)
    temperature, humidity = random_temperature_humidity()
    if ngsi_update(ip, lab, temperature, humidity):
        print('Temperature: {} // Humidity: {}'.format(temperature, humidity))
    else:
        print('FAILED UPDATE')

def simulated_arduino(fiware_ip, fiware_lab):
    global ip, lab
    ip = fiware_ip
    lab = fiware_lab

    setup()
    while True:
        loop()

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('ip')
    parser.add_argument('lab')
    args = parser.parse_args()
    simulated_arduino(args.ip, args.lab)
