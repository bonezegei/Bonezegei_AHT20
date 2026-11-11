/*
  Read AHT20 Sensor value
  Author: Jofel Batutay (Bonezegei)
*/
#include <Wire.h>
#include <Bonezegei_AHT20.h>

// Initialize AHT20 with default I2C address (0x38)
Bonezegei_AHT20 aht20(0x38);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!aht20.begin()) {
    Serial.println("Could not find a valid AHT20 sensor, check wiring!");
    while (1);
  }

  Serial.println("AHT20 Sensor Initialized!");
}

void loop() {
  if (aht20.read()) {
    Serial.print("Temperature: ");
    Serial.print(aht20.getTemperature());
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(aht20.getHumidity());
    Serial.println(" %");

    Serial.println("------------------------------------");
  } else {
    Serial.println("Failed to read from AHT20 sensor!");
  }

  delay(2000);
}