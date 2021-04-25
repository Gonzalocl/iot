
import argparse
import requests

template_url = 'http://{}:1026/v1/subscribeContext'
template_payload = {
    'entities': [
        {
            'type': 'Laboratorio',
            'isPattern': 'false',
            'id': ''
        }
    ],
    'attributes': [
        'humidity'
    ],
    'reference': 'http://localhost:1111/alerta',
    'duration': 'P1M',
    'notifyConditions': [
        {
            'type': 'ONCHANGE',
            'condValues': [
                'temperature'
            ]
        }
    ],
    'throttling': 'PT5S'
}

def subscribe_lab(ip, lab):
    pass

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('ip')
    parser.add_argument('lab')
    args = parser.parse_args()
