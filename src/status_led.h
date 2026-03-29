/**
 * @file status_led.h
 * @brief Interface for the on-board status LED used to indicate WiFi state.
 */
#pragma once

/**
 * @brief Configure the status LED GPIO as an output and turn the LED off.
 */
void initStatusLed();

/**
 * @brief Directly set the status LED on or off.
 *
 * @param on true to illuminate the LED, false to extinguish it.
 */
void setStatusLed(bool on);

/**
 * @brief Update the status LED to reflect the current WiFi connection state.
 *
 * Holds the LED on solid when WiFi is disconnected; blinks at
 * STATUS_LED_BLINK_MS intervals when connected. Call every loop iteration.
 */
void updateStatusLed();
