/**
 * @file provisioning.h
 * @brief Interface for BLE-based WiFi provisioning via the Espressif WiFiProv library.
 */
#pragma once
#include <Arduino.h>
#include <WiFi.h>

/**
 * @brief Arduino WiFi event handler for provisioning lifecycle events.
 *
 * Prints status messages to Serial for each provisioning stage and triggers a
 * restart on credential failure.
 *
 * @param event Pointer to the Arduino system event structure delivered by the WiFi stack.
 */
void onProvEvent(arduino_event_t* event);

/**
 * @brief Start BLE provisioning and block until WiFi credentials are accepted.
 *
 * Checks the REPROV_BTN_PIN jumper; if held low, forces re-provisioning by
 * clearing existing credentials. After provisioning completes, waits for the
 * jumper to be released before rebooting into normal operation.
 */
void runProvisioning();
