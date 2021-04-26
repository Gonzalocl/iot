
import argparse
import requests
from pprint import pprint

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
    'reference': 'http://192.168.1.33:1111/alerta',
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

    url = template_url.format(ip)
    template_payload['entities'][0]['id'] = 'Laboratorio{}'.format(lab)
    if lab == '*':
        template_payload['entities'][0]['isPattern'] = 'true'

    response = requests.post(url, json=template_payload).json()

    try:
        if not 'subscribeResponse' in response:
            return response, None

        return response, response['subscribeResponse']['subscriptionId']

    except KeyError:
        return response, None

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('ip')
    parser.add_argument('lab')
    parser.add_argument('--print-json', default=False, action='store_true')
    args = parser.parse_args()

    json, subscription_id = subscribe_lab(args.ip, args.lab)

    if not subscription_id:
        print('ERROR')
        pprint(json)
        exit(1)

    print('subscriptionId: {}'.format(subscription_id))

    if args.print_json:
        pprint(json)