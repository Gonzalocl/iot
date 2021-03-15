#include<DHT.h>

#define DHT_PIN 3
#define FAN_PIN 8
#define LED_PIN 9

DHT dht(DHT_PIN, DHT11);
float temperature_threshold, humidity_threshold;
bool fan_on, led_on;

void setup() {
  Serial.begin(9600);
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
  delay(5000);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }

  print_sensor_info(temperature, humidity);
  check_sensor_info(temperature, humidity);
  
}
