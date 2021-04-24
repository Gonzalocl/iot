
import argparse
import requests
from pprint import pprint

template_url = 'http://{}:1026/v1/queryContext'
template_payload = {
    'entities': [
        {
            'type': 'Laboratorio',
            'isPattern': 'false',
            'id': ''
        }
    ]
}

def query_lab(ip, lab):
    url = template_url.format(ip)
    template_payload['entities'][0]['id'] = 'Laboratorio{}'.format(lab)
    if lab == '*':
        template_payload['entities'][0]['isPattern'] = 'true'
    response = requests.post(url, json=template_payload).json()
    try:
        if not 'contextResponses' in response:
            return response, None
        data = []
        for r in response['contextResponses']:
            element_id = r['contextElement']['id']
            temperature = None
            humidity = None
            for a in r['contextElement']['attributes']:
                if a['name'] == 'temperature':
                    temperature = a['value']
                elif a['name'] == 'humidity':
                    humidity = a['value']
            data.append({'element_id': element_id, 'temperature': temperature, 'humidity': humidity})
        return response, data
    except KeyError:
        return response, None

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('ip')
    parser.add_argument('lab')
    parser.add_argument('--print-json', default=False, action='store_true')
    args = parser.parse_args()
    json, data = query_lab(args.ip, args.lab)

    if not data:
        print('ERROR')
        pprint(json)
        exit(1)

    for d in data:
        print('{} :: Temperature: {} ºC | Humidity: {} %'.format(d['element_id'],
                                                                 d['temperature'],
                                                                 d['humidity']))

    if args.print_json:
        pprint(json)
