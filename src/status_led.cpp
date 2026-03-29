/**
 * @file status_led.cpp
 * @brief Status LED implementation: solid when WiFi is down, blinking when connected.
 */
#include "status_led.h"
#include "config.h"
#include <Arduino.h>
#include <WiFi.h>

void initStatusLed() {
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, STATUS_LED_OFF);
}

void setStatusLed(bool on) {
  digitalWrite(STATUS_LED_PIN, on ? STATUS_LED_ON : STATUS_LED_OFF);
}

void updateStatusLed() {
  static uint32_t lastBlinkMs = 0; ///< Timestamp of the last blink toggle
  static bool     ledOn       = false; ///< Current blink state

  if (WiFi.status() != WL_CONNECTED) {
    setStatusLed(true);
    return;
  }

  if (millis() - lastBlinkMs >= STATUS_LED_BLINK_MS) {
    lastBlinkMs = millis();
    ledOn = !ledOn;
    setStatusLed(ledOn);
  }
}
