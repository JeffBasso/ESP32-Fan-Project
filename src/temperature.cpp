/**
 * @file temperature.cpp
 * @brief DS18B20 one-wire temperature sensor implementation.
 */
#include "temperature.h"
#include "config.h"
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

static OneWire           oneWire(TEMP_PROBE_PIN);    ///< One-wire bus on TEMP_PROBE_PIN
static DallasTemperature tempSensor(&oneWire);        ///< DS18B20 library instance

void initTemp() {
  tempSensor.begin();
  int count = tempSensor.getDeviceCount();
  if (count == 0) {
    Serial.println("DS18B20 not found");
  } else {
    Serial.printf("DS18B20 sensors found: %d\n", count);
  }
}

float getProbeTemp() {
  tempSensor.requestTemperatures();
  float c = tempSensor.getTempCByIndex(0);
  if (c == DEVICE_DISCONNECTED_C) return NAN;
  return c;
}

float toFahrenheit(float c) {
  return c * 9.0f / 5.0f + 32.0f;
}
