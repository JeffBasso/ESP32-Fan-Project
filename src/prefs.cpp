/**
 * @file prefs.cpp
 * @brief Implementation of NVS-backed load/save for fan controller settings.
 */
#include "prefs.h"
#include "globals.h"
#include <Arduino.h>
#include <Preferences.h>

static Preferences prefs; ///< ESP32 NVS Preferences instance scoped to this module

void loadPrefs() {
  prefs.begin("fan", true);
  targetTempF    = prefs.getFloat("targetF", 75.0f);
  runFullOnError = prefs.getBool ("runFull",  false);
  Kp             = prefs.getFloat("kp",       2.0f);
  Ki             = prefs.getFloat("ki",       0.1f);
  Kd             = prefs.getFloat("kd",       0.0f);
  prefs.end();
  Serial.printf("Loaded: target=%.1fF  Kp=%.2f Ki=%.3f Kd=%.3f  runFull=%s\n",
                targetTempF, Kp, Ki, Kd, runFullOnError ? "ON" : "OFF");
}

void savePrefs() {
  prefs.begin("fan", false);
  prefs.putFloat("targetF",  targetTempF);
  prefs.putBool ("runFull",  runFullOnError);
  prefs.putFloat("kp",       (float)Kp);
  prefs.putFloat("ki",       (float)Ki);
  prefs.putFloat("kd",       (float)Kd);
  prefs.end();
}
