/**
 * @file main.cpp
 * @brief Application entry point: setup and main control loop for the seedling fan controller.
 *
 * Initialises all subsystems, connects to WiFi via BLE provisioning, then
 * runs a periodic loop that reads temperature, drives the PID controller,
 * updates the fan, and pushes telemetry to the ESPDash web dashboard.
 */
#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "globals.h"
#include "prefs.h"
#include "fan.h"
#include "temperature.h"
#include "pid_control.h"
#include "status_led.h"
#include "dashboard.h"
#include "provisioning.h"

/**
 * @brief One-time initialisation: hardware, preferences, PID, WiFi, and dashboard.
 */
void setup() {
  Serial.begin(115200);
  delay(3000);
  initStatusLed();
  setStatusLed(true);

  initFan();
  initTemp();
  loadPrefs();

  fanPID.SetTunings(Kp, Ki, Kd);

  float initialTemp = toFahrenheit(getProbeTemp());
  initPID(isnan(initialTemp) ? targetTempF : initialTemp);

  runProvisioning();

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    setStatusLed(true);
  }
  Serial.printf("\nConnected. Open http://%s in Safari, then Add to Home Screen.\n",
                WiFi.localIP().toString().c_str());

  initDashboard();
  server.begin();
}

/**
 * @brief Main control loop: runs every CONTROL_INTERVAL_MS milliseconds.
 *
 * Reads the probe temperature, handles disconnected-probe failsafe (fan full or
 * off per runFullOnError), runs the PID update, and pushes changed telemetry to
 * the dashboard. The status LED is refreshed on every iteration regardless of
 * the control interval.
 */
void loop() {
  static uint32_t lastControlUpdate = 0;  ///< Timestamp of the last control cycle
  static float    lastPrintedTempF  = -999.0f; ///< Last temperature sent to the dashboard
  static int      lastPrintedFanPct = -1; ///< Last fan percent sent to the dashboard
  static int      lastPrintedRpm    = -1; ///< Last RPM value sent to the dashboard
  static bool     probeWasLost      = false; ///< Tracks probe-lost state to avoid repeated dashboard updates

  updateStatusLed();

  if (millis() - lastControlUpdate < CONTROL_INTERVAL_MS) return;
  lastControlUpdate = millis();

  float tempC = getProbeTemp();

  if (isnan(tempC)) {
    if (!probeWasLost) {
      Serial.println("Probe: disconnected");
      cardStatus.setFeedback("Disconnected", dash::Status::DANGER);
      dashboard.sendUpdates();
      probeWasLost     = true;
      lastPrintedTempF = NAN;
    }
    setFanPercent(runFullOnError ? 100 : 0); ///< sets fan to 100% or 0 if browser flag is set
    return;
  }

  if (probeWasLost) {
    cardStatus.setFeedback("Connected", dash::Status::SUCCESS);
    dashboard.sendUpdates();
    probeWasLost = false;
  }

  float tempF = toFahrenheit(tempC);
  updatePID(tempF);

  int rpm    = getFanRPM();
  int fanPct = (int)pidOutput;

  bool changed = (roundf(tempF * 10) != roundf(lastPrintedTempF * 10)) ||
                 (fanPct != lastPrintedFanPct) ||
                 (rpm    != lastPrintedRpm);

  if (changed) {
    cardTemp.setValue(tempF);
    cardFanPct.setValue(fanPct);
    cardRpm.setValue(rpm);
    dashboard.sendUpdates();

    Serial.printf("Temp: %.1f F  |  Target: %.1f F  |  Fan: %d%%  |  RPM: %d  |  Kp=%.2f Ki=%.3f Kd=%.3f\n",
                  tempF, targetTempF, fanPct, rpm, Kp, Ki, Kd);
    lastPrintedTempF  = tempF;
    lastPrintedFanPct = fanPct;
    lastPrintedRpm    = rpm;
  }
}
