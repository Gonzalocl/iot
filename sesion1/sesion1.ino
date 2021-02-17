#include<DHT.h>

#define DHTPIN 3

DHT dht(DHTPIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
  float humidity_threshold = dht.readHumidity() + 2;
  float temperature_threshold = dht.readTemperature() + 2;
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
}
