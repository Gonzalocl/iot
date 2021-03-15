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

void print_sensor_info(float temperature, float humidity) {
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" ºC");
  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.println(" %");
}

void check_sensor_info(float temperature, float humidity) {
  
  if (temperature >= temperature_threshold && !fan_on) {
    // turn on fan
    Serial.println("Encendido automatico ventilador");
    digitalWrite(FAN_PIN, HIGH);
    fan_on = true;
  } else if (temperature < temperature_threshold && fan_on) {
    // turn off fan
    Serial.println("Apagado automatico ventilador");
    digitalWrite(FAN_PIN, LOW);
    fan_on = false;
  }

  if (humidity >= humidity_threshold && !led_on) {
    // turn on led
    Serial.println("Encendido automatico LED");
    digitalWrite(LED_PIN, HIGH);
    led_on = true;
  } else if (humidity < humidity_threshold && led_on) {
    // turn off led
    Serial.println("Apagado automatico LED");
    digitalWrite(LED_PIN, LOW);
    led_on = false;
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

  print_sensor_info(temperature, humidity);
//  check_sensor_info(temperature, humidity);

  thing_relay.handle();
  thing_dht.handle();
  
}
