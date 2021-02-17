#include<DHT.h>

#define DHT_PIN 3
#define FAN_PIN 8
#define LED_PIN 9

DHT dht(DHT_PIN, DHT11);
float humidity_threshold, temperature_threshold;

void setup() {
  Serial.begin(9600);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  humidity_threshold = dht.readHumidity() + 2;
  temperature_threshold = dht.readTemperature() + 2;
}

void loop() {
  delay(5000);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
  
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" ºC");
  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.println(" %");

  if (temperature >= temperature_threshold) {
    // turn on fan
    digitalWrite(FAN_PIN, HIGH);
  } else {
    // turn off fan
    digitalWrite(FAN_PIN, LOW);
  }

  if (humidity >= humidity_threshold) {
    // turn on led
    digitalWrite(LED_PIN, HIGH);
  } else {
    // turn off led
    digitalWrite(LED_PIN, LOW);
  }
  
}
