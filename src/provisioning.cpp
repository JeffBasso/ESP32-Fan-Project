/**
 * @file provisioning.cpp
 * @brief BLE WiFi provisioning implementation using the Espressif WiFiProv library.
 */
#include "provisioning.h"
#include "config.h"
#include <WiFi.h>
#include <WiFiProv.h>

void onProvEvent(arduino_event_t* event) {
  switch (event->event_id) {
    case ARDUINO_EVENT_PROV_START:
      Serial.println("Provisioning: BLE active");
      Serial.printf("  App:    ESP BLE Provisioning (Espressif)\n");
      Serial.printf("  Device: %s\n", PROV_SERVICE_NAME);
      Serial.printf("  PIN:    %s\n", PROV_POP);
      break;
    case ARDUINO_EVENT_PROV_CRED_RECV:
      Serial.println("Provisioning: WiFi credentials received");
      break;
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
      Serial.println("Provisioning: credentials accepted");
      break;
    case ARDUINO_EVENT_PROV_CRED_FAIL:
      Serial.println("Provisioning: failed — restarting");
      delay(1000);
      ESP.restart();
      break;
    case ARDUINO_EVENT_PROV_END:
      Serial.println("Provisioning: complete — pull jumper on D3 to reboot");
      break;
    default:
      break;
  }
}

void runProvisioning() {
  pinMode(REPROV_BTN_PIN, INPUT_PULLUP);
  bool forceReprovision = (digitalRead(REPROV_BTN_PIN) == LOW);
  if (forceReprovision) Serial.println("Reprovision jumper held — wiping WiFi credentials");

  Serial.println("Starting BLE provisioning...");
  WiFi.onEvent(onProvEvent);
  WiFiProv.beginProvision(
    NETWORK_PROV_SCHEME_BLE,
    NETWORK_PROV_SCHEME_HANDLER_FREE_BLE,
    NETWORK_PROV_SECURITY_1,
    PROV_POP,
    PROV_SERVICE_NAME,
    NULL, NULL,
    forceReprovision
  );

  if (forceReprovision) {
    Serial.println("Waiting for jumper pull on D3 to reboot...");
    while (digitalRead(REPROV_BTN_PIN) == LOW) {
      delay(100);
    }
    Serial.println("Jumper pulled — rebooting to normal mode");
    delay(500);
    ESP.restart();
  }
}
