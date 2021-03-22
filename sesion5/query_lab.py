
import argparse
import requests

template_url = 'http://{}:1026/v1/queryContext'
template_payload = {
    'entities': [
        {
            'type': 'Laboratorio',
            'isPattern': 'false'
        }
    ]
}

def query_lab(ip, lab):
    url = template_url.format(ip)
    template_payload['entities'][0]['id'] = 'Laboratorio{}'.format(lab)
    response = requests.post(url, json=template_payload)
    return response.json()
    # return temperature, humidity

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('ip')
    parser.add_argument('lab')
    args = parser.parse_args()
    print(query_lab(args.ip, args.lab))
    # temperature, humidity = query_lab(args.ip, args.lab)
    # print('Temperature: {}'.format(temperature))
    # print('Humidity: {}'.format(humidity))
