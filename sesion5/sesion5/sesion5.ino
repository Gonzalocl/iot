#include<DHT.h>

#include <SPI.h>
#include <Ethernet.h>

#define DHT_PIN 3
#define LAB "1"

#define UPDATE_CHUNK_0 "POST /v1/updateContext HTTP/1.1\r\nHost: localhost:1026\r\nContent-Type: application/json\r\nAccept: application/json\r\nContent-Length: "
#define UPDATE_CHUNK_1 "\r\nConnection: close\r\n\r\n{\"contextElements\":[{\"type\":\"Laboratorio\",\"isPattern\":\"false\",\"id\":\"Laboratorio"
#define UPDATE_CHUNK_2 "\",\"attributes\":[{\"name\":\"temperature\",\"type\":\"float\",\"value\":"
#define UPDATE_CHUNK_3 "},{\"name\":\"humidity\",\"type\":\"float\",\"value\":"
#define UPDATE_CHUNK_4 "}]}],\"updateAction\":\""
#define UPDATE_CHUNK_5 "\"}"

#define UPDATE_CONSTANT_LENGTH 214
#define FLOAT_CONSTANT_LENGTH 3

DHT dht(DHT_PIN, DHT11);

byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x80, 0xAA };
IPAddress ip(10, 1, 1, 2);
IPAddress dns(192, 168, 1, 1);
char const * fiware_server = "yellow";
int fiware_port = 1026;
EthernetClient client;

int get_float_length(float f) {
  return FLOAT_CONSTANT_LENGTH;
}

int get_content_length(float temperature, float humidity) {
  return UPDATE_CONSTANT_LENGTH + get_float_length(temperature) + get_float_length(humidity);
}

bool ngsi_send_request(float temperature, float humidity, char const * update_action) {

  if (!client.connect(fiware_server, fiware_port)) {
    return false;
  }

  client.print(UPDATE_CHUNK_0);
  client.print(get_content_length(temperature, humidity));
  client.print(UPDATE_CHUNK_1 LAB UPDATE_CHUNK_2);
  client.print(temperature);
  client.print(UPDATE_CHUNK_3);
  client.print(humidity);
  client.print(UPDATE_CHUNK_4);
  client.print(update_action);
  client.print(UPDATE_CHUNK_5);

  client.stop();
  return true;

}

void start_ethernet() {

  // start Ethernet
  Ethernet.begin(mac, ip, dns);

  // check hardware
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Error: no Ethernet port");
    while (true) {
      delay(1000);
    }
  }

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Error: no Ethernet cable");
  }

}

void print_sensor_info(float temperature, float humidity) {

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" ºC | ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

}

void setup() {

  Serial.begin(9600);
  while(!Serial);

  dht.begin();

  start_ethernet();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error: read DHT11 data");
    return;
  }

  print_sensor_info(temperature, humidity);

  ngsi_send_request(temperature, humidity, "APPEND");

}

void loop() {

  delay(5000);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error: read DHT11 data");
    return;
  }

  print_sensor_info(temperature, humidity);

  ngsi_send_request(temperature, humidity, "UPDATE");

}
