#include<DHT.h>

#define DHT_PIN 3

DHT dht(DHT_PIN, DHT11);

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

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }

  print_sensor_info(temperature, humidity);

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
  
}
