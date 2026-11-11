/*
  Bonezegei_AHT20.h - Arduino Library for AHT20 Sensor
  Author: Jofel Batutay (Bonezegei)
  Date: November 11, 2026
*/

#ifndef BONEZEGEI_AHT20_H
#define BONEZEGEI_AHT20_H

#include <Arduino.h>
#include <Wire.h>

#define AHT20_I2C_ADDR 0x38

// The AHT20 uses 0xBE for initialization/calibration, unlike the AHT10 which uses 0xE1
#define AHT20_CMD_INITIALIZE 0xBE
#define AHT20_CMD_MEASURE    0xAC
#define AHT20_CMD_SOFT_RESET 0xBA

class Bonezegei_AHT20 {
public:
  Bonezegei_AHT20(uint8_t addr = AHT20_I2C_ADDR);
  
  bool begin(TwoWire &wirePort = Wire);
  bool read();
  
  float getTemperature();  // Returns Temperature in Celsius
  float getHumidity();     // Returns Relative Humidity in %

private:
  uint8_t _addr;
  TwoWire *_wire;

  float _temperature;
  float _humidity;

  uint8_t readStatus();
};

#endif