
import argparse
import requests
import time

ip = ''
lab = ''

def random_temperature_humidity():
    return 10, 15

def setup():
    pass

def loop():

    time.sleep(5)

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
