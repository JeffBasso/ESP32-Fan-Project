/**
 * @file fan.h
 * @brief Interface for initializing and controlling the EMC2101 PWM fan driver.
 */
#pragma once

/**
 * @brief Initialize the EMC2101 fan controller over I2C.
 *
 * Enables tachometer input, configures the PWM clock for ~25 kHz, disables the
 * hardware look-up table, and sets the initial duty cycle to 0%.
 */
void initFan();

/**
 * @brief Set the fan speed as a percentage of full speed.
 *
 * Maps the requested percentage to the EMC2101 duty-cycle register range and
 * clamps the value to [0, 100].
 *
 * @param pct Desired fan speed in percent (0 = off, 100 = full speed).
 */
void setFanPercent(int pct);

/**
 * @brief Read the current fan speed from the EMC2101 tachometer.
 *
 * @return Fan speed in RPM as reported by the EMC2101.
 */
int  getFanRPM();
