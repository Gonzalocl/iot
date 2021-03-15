#include<DHT.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ThingerEthernet.h>

#include "credentials.h"

#define DHT_PIN 3
#define FAN_PIN 8
#define LED_PIN 9

DHT dht(DHT_PIN, DHT11);
float temperature, humidity, temperature_threshold, humidity_threshold;
bool fan_on, led_on;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x80, 0xAA };
IPAddress ip(10, 1, 1, 2);
IPAddress dns(192, 168, 1, 1);

ThingerEthernet thing_relay(USERNAME, relay_ID, relay_CREDENTIAL);
ThingerEthernet thing_dht(USERNAME, dht_ID, dht_CREDENTIAL);

void start_ethernet() {

  // start Ethernet
  Ethernet.begin(mac, ip, dns);

  // check hardware
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("No hay puerto Ethernet");
    while (true) {
      delay(1000);
    }
  }
  
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Cable Ethernet no conectado");
  }

}

void setup() {
  
  Serial.begin(9600);
  dht.begin();
  
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  temperature_threshold = dht.readTemperature() + 2;
  humidity_threshold = dht.readHumidity() + 2;
  fan_on = false;
  led_on = false;
  
  Serial.print("Umbral temperatura: ");
  Serial.print(temperature_threshold);
  Serial.println(" ºC");
  Serial.print("Umbral humedad: ");
  Serial.print(humidity_threshold);
  Serial.println(" %");

  start_ethernet();

  thing_relay["relay"] << digitalPin(FAN_PIN);
  thing_dht["dht"] >> [](pson& out){
    out["temperature"] = temperature;
    out["humidity"] = humidity;
  };
  
}

void print_sensor_info() {
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" ºC");
  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.println(" %");
}

void check_sensor_info() {
  
  if (temperature >= temperature_threshold || humidity >= humidity_threshold) {
    Serial.println("warning");
    pson data;
    data["temperature"] = temperature;
    data["humidity"] = humidity;
    thing_dht.call_endpoint(dht_ENDPOINT_ID, data);
  }
  
}

void loop() {
  delay(1000);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }

  print_sensor_info();
  check_sensor_info();

  thing_relay.handle();
  thing_dht.handle();
  
}
