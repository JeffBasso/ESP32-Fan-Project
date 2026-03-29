/**
 * @file config.h
 * @brief Compile-time hardware pin assignments, timing constants, and BLE provisioning settings.
 */
#pragma once

// ── Hardware pins ─────────────────────────────────────────────────────────────
#define TEMP_PROBE_PIN   3    ///< GPIO pin for DS18B20 one-wire data line (D2 on XIAO ESP32-S3)
#define REPROV_BTN_PIN   4    ///< GPIO pin for re-provisioning jumper (D3 on XIAO ESP32-S3 — wire to GND, hold on reset to re-provision)
#define STATUS_LED_PIN   LED_BUILTIN ///< GPIO pin for the on-board status LED
#define STATUS_LED_ON    LOW  ///< Logic level that turns the status LED on (active-low)
#define STATUS_LED_OFF   HIGH ///< Logic level that turns the status LED off

// ── Timing ────────────────────────────────────────────────────────────────────
#define CONTROL_INTERVAL_MS   2000 ///< Milliseconds between each temperature-read and PID update cycle
#define STATUS_LED_BLINK_MS   500  ///< Milliseconds per blink half-period when WiFi is connected

// ── WiFi Provisioning ─────────────────────────────────────────────────────────
// WiFi credentials are NOT stored here — provisioned at runtime via BLE.
// App: "ESP BLE Provisioning" by Espressif (iOS/Android)
// To provision / re-provision: hold jumper on D3 during reset
// PROV_SERVICE_NAME and PROV_POP are defined in config_local.h (gitignored).
// Copy src/config_local.h.example to src/config_local.h and set your values.
#include "config_local.h"
