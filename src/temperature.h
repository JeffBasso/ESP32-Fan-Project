/**
 * @file temperature.h
 * @brief Interface for the DS18B20 one-wire temperature probe.
 */
#pragma once

/**
 * @brief Initialize the DS18B20 sensor bus and report the device count to Serial.
 */
void  initTemp();

/**
 * @brief Trigger a temperature conversion and return the raw Celsius reading.
 *
 * @return Temperature in degrees Celsius, or NAN if the probe is disconnected.
 */
float getProbeTemp();

/**
 * @brief Convert a Celsius temperature to Fahrenheit.
 *
 * @param c Temperature in degrees Celsius.
 * @return Equivalent temperature in degrees Fahrenheit.
 */
float toFahrenheit(float c);
