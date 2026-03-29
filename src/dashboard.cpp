/**
 * @file dashboard.cpp
 * @brief ESPDash web dashboard setup: cards, sliders, and live update callbacks.
 */
#include "dashboard.h"
#include "globals.h"
#include "pid_control.h"
#include "prefs.h"
#include <Arduino.h>

AsyncWebServer server(80);                        ///< HTTP server on port 80
ESPDash        dashboard(server, "/dash", true);  ///< Dashboard mounted at /dash

/// Minimal HTML wrapper served at "/" that fetches and injects the dashboard,
/// with iOS home-screen meta tags so it can be saved as a web app.
static const char IOS_WRAPPER[] =
  "<!DOCTYPE html><html><head>"
  "<meta charset='UTF-8'>"
  "<meta name='viewport' content='width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no'>"
  "<meta name='apple-mobile-web-app-capable' content='yes'>"
  "<meta name='apple-mobile-web-app-status-bar-style' content='black'>"
  "<meta name='apple-mobile-web-app-title' content='Fan Controller'>"
  "<title>Fan Controller</title>"
  "<script>"
  "fetch('/dash')"
  ".then(function(r){return r.text();})"
  ".then(function(h){document.open();document.write(h);document.close();});"
  "</script>"
  "</head><body></body></html>";

// ── Public telemetry cards ────────────────────────────────────────────────────
dash::TemperatureCard<float> cardTemp   (dashboard, "Temperature", "F"); ///< Current probe temperature
dash::GenericCard<int>       cardFanPct (dashboard, "Fan",         "%"); ///< Current fan duty cycle
dash::GenericCard<int>       cardRpm    (dashboard, "RPM",         "rpm"); ///< Current fan RPM
dash::FeedbackCard<>         cardStatus (dashboard, "Probe");             ///< Probe connection status

// ── Settings controls (private to the dashboard) ─────────────────────────────
static dash::SliderCard<int>   sliderTarget (dashboard, "Target Temp", 60,   90,   1,     "F"); ///< Target temperature setpoint slider (°F, 60–90)
static dash::SliderCard<float> sliderKp     (dashboard, "Kp",          0.0f, 10.0f, 0.1f, ""); ///< PID Kp slider (0–10)
static dash::SliderCard<float> sliderKi     (dashboard, "Ki",          0.0f, 0.5f,  0.01f,""); ///< PID Ki slider (0–0.5)
static dash::SliderCard<float> sliderKd     (dashboard, "Kd",          0.0f, 2.0f,  0.1f, ""); ///< PID Kd slider (0–2)
static dash::ToggleButtonCard  btnRunFull   (dashboard, "Run Full on Error"); ///< Toggle: run fan at 100% when probe is lost

void initDashboard() {
  sliderTarget.setValue((int)targetTempF);
  sliderKp.setValue((float)Kp);
  sliderKi.setValue((float)Ki);
  sliderKd.setValue((float)Kd);
  runFullOnError ? btnRunFull.on() : btnRunFull.off();
  cardStatus.setFeedback("Connected", dash::Status::SUCCESS);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", IOS_WRAPPER);
  });

  sliderTarget.onChange([](int val) {
    targetTempF = (float)val;
    pidSetpoint = targetTempF;
    sliderTarget.setValue(val);
    dashboard.sendUpdates();
    savePrefs();
    Serial.printf("Target temp set to %.1f F\n", targetTempF);
  });

  sliderKp.onChange([](float val) {
    Kp = val;
    fanPID.SetTunings(Kp, Ki, Kd);
    sliderKp.setValue(val);
    dashboard.sendUpdates();
    savePrefs();
    Serial.printf("Kp set to %.2f\n", Kp);
  });

  sliderKi.onChange([](float val) {
    Ki = val;
    fanPID.SetTunings(Kp, Ki, Kd);
    sliderKi.setValue(val);
    dashboard.sendUpdates();
    savePrefs();
    Serial.printf("Ki set to %.3f\n", Ki);
  });

  sliderKd.onChange([](float val) {
    Kd = val;
    fanPID.SetTunings(Kp, Ki, Kd);
    sliderKd.setValue(val);
    dashboard.sendUpdates();
    savePrefs();
    Serial.printf("Kd set to %.3f\n", Kd);
  });

  btnRunFull.onChange([](bool state) {
    runFullOnError = state;
    btnRunFull.setValue(state);
    dashboard.sendUpdates();
    savePrefs();
    Serial.printf("Run full on error: %s\n", state ? "ON" : "OFF");
  });
}
