#include<DHT.h>

#include <SPI.h>
#include <Ethernet.h>

#define DHT_PIN 3
#define FAN_PIN 8
#define LED_PIN 9
#define HISTORY_SIZE 10

DHT dht(DHT_PIN, DHT11);
float temperature_threshold, humidity_threshold;
float temperature_history[HISTORY_SIZE];
float humidity_history[HISTORY_SIZE];
int history_index = 0;
bool fan_on, led_on;

byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0x80, 0xAA
};
IPAddress ip(169, 254, 139, 190);
EthernetServer server(80);

void start_web_server() {

  // start Ethernet
  Ethernet.begin(mac, ip);

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

  // start server
  server.begin();
  Serial.print("Server ip: ");
  Serial.println(Ethernet.localIP());
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
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
  
  for (int i = 0; i < HISTORY_SIZE; i++) {
    temperature_history[i] = 0;
    humidity_history[i] = 0;
  }

  start_web_server();
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

String web_html = "HTTP/1.1 200 OK\n\rContent-Type: text/html\n\rConnection: close\n\r\n\r<!DOCTYPE HTML><html>Test 0</html>";

void send_web(EthernetClient client) {
  client.print(web_html);
}

void web_server() {
  EthernetClient client = server.available();
  if (client) {
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        // read char by char
        char c = client.read();
        if (c == '\n' && blank_line) {
          send_web(client);
        }
        if (c == '\n') {
          blank_line = true;
        } else if (c != '\r') {
          blank_line = false;
        }
      } else {
        delay(1);
        client.stop();
      }
    }
  }
}

void loop() {
  delay(1000);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }

  print_sensor_info(temperature, humidity);
  check_sensor_info(temperature, humidity);

  temperature_history[history_index] = temperature;
  humidity_history[history_index] = humidity;
  history_index = (history_index+1) % HISTORY_SIZE;

  web_server();
}
