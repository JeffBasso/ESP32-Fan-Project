/**
 * @file fan.cpp
 * @brief EMC2101 fan driver implementation.
 */
#include "fan.h"
#include <Arduino.h>
#include <Adafruit_EMC2101.h>

static Adafruit_EMC2101 emc2101; ///< EMC2101 driver instance

void initFan() {
  if (!emc2101.begin()) {
    Serial.println("EMC2101 not found");
  } else {
    emc2101.enableTachInput(true);
    emc2101.configPWMClock(false, false);
    emc2101.setPWMFrequency(0x07);  // ~25 kHz
    emc2101.LUTEnabled(false);
    setFanPercent(0);
    Serial.println("EMC2101 ready");
  }
}

void setFanPercent(int pct) {
  if (pct <= 0) { emc2101.setDutyCycle(0); return; }
  if (pct > 100) pct = 100;
  int duty = 5 + (pct * 19) / 100;
  emc2101.setDutyCycle(duty);
}

int getFanRPM() {
  return emc2101.getFanRPM();
}
