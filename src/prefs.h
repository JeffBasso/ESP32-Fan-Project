/**
 * @file prefs.h
 * @brief Interface for loading and saving persistent user settings via NVS.
 */
#pragma once

/**
 * @brief Load all user-configurable settings from NVS into global variables.
 *
 * Opens the "fan" NVS namespace in read-only mode, reads each stored value
 * (or its default), and prints a summary to Serial.
 */
void loadPrefs();

/**
 * @brief Persist all current global settings to NVS.
 *
 * Opens the "fan" NVS namespace in read-write mode and writes targetTempF,
 * runFullOnError, Kp, Ki, and Kd.
 */
void savePrefs();
