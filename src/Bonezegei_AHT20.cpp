/*
  Bonezegei_AHT20.cpp - Arduino Library for AHT20 Sensor
  Author: Jofel Batutay (Bonezegei)
  Date: November 11, 2026
*/

#include "Bonezegei_AHT20.h"

Bonezegei_AHT20::Bonezegei_AHT20(uint8_t addr) {
  _addr = addr;
  _temperature = 0.0;
  _humidity = 0.0;
}

bool Bonezegei_AHT20::begin(TwoWire &wirePort) {
  _wire = &wirePort;
  _wire->begin();
  delay(40); // Wait for sensor to power up (AHT20 recommended wait time)

  // Check if calibrated (Bit 3 of status byte should be 1)
  if ((readStatus() & 0x08) == 0) {
    // Initialize/Calibrate if not already calibrated
    _wire->beginTransmission(_addr);
    _wire->write(AHT20_CMD_INITIALIZE);
    _wire->write(0x08);
    _wire->write(0x00);
    if (_wire->endTransmission() != 0) {
      return false; // Device not found or I2C error
    }
    delay(10);
  }

  return true;
}

bool Bonezegei_AHT20::read() {
  // Trigger Measurement
  _wire->beginTransmission(_addr);
  _wire->write(AHT20_CMD_MEASURE);
  _wire->write(0x33);
  _wire->write(0x00);
  if (_wire->endTransmission() != 0) return false;

  // Wait for measurement to complete (Busy bit 7 goes to 0)
  uint8_t timeout = 100;
  while ((readStatus() & 0x80) != 0) {
    delay(5);
    if (--timeout == 0) return false;
  }

  _wire->requestFrom(_addr, (uint8_t)7); // AHT20 standard read requests 7 bytes (status + 6 data)
  if (_wire->available() < 6) return false;

  uint8_t data[7];
  for (int i = 0; i < 7; i++) {
    data[i] = _wire->read();
  }

  // Convert Humidity (20 bits)
  // data[1] is hum MSB, data[2] is hum LSB, data[3] high nibble is hum XLSB
  uint32_t hum_raw = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
  _humidity = ((float)hum_raw * 100.0f) / 1048576.0f; // 1048576 = 2^20

  // Convert Temperature (20 bits)
  // data[3] low nibble is temp MSB, data[4] is temp LSB, data[5] is temp XLSB
  uint32_t temp_raw = (((uint32_t)(data[3] & 0x0F)) << 16) | ((uint32_t)data[4] << 8) | data[5];
  _temperature = (((float)temp_raw * 200.0f) / 1048576.0f) - 50.0f;

  return true;
}

float Bonezegei_AHT20::getTemperature() { return _temperature; }
float Bonezegei_AHT20::getHumidity() { return _humidity; }

uint8_t Bonezegei_AHT20::readStatus() {
  _wire->requestFrom(_addr, (uint8_t)1);
  if (_wire->available()) {
    return _wire->read();
  }
  return 0;
}