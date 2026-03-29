/**
 * @file dashboard.h
 * @brief Interface for the ESPDash web dashboard and its public card widgets.
 *
 * Exposes the AsyncWebServer, ESPDash instance, and the four read-only data
 * cards so that main.cpp can push telemetry updates each control cycle.
 */
#pragma once
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>

extern AsyncWebServer              server;     ///< HTTP server serving the dashboard on port 80
extern ESPDash                     dashboard;  ///< ESPDash instance bound to the server
extern dash::TemperatureCard<float> cardTemp;  ///< Displays the current probe temperature
extern dash::GenericCard<int>       cardFanPct; ///< Displays the current fan duty cycle in percent
extern dash::GenericCard<int>       cardRpm;   ///< Displays the current fan speed in RPM
extern dash::FeedbackCard<>         cardStatus; ///< Displays the probe connection status

/**
 * @brief Register all dashboard cards, sliders, and change-event callbacks, and
 *        add the iOS home-screen wrapper route to the web server.
 *
 * Must be called after WiFi is connected and before server.begin().
 */
void initDashboard();
