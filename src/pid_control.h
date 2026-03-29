/**
 * @file pid_control.h
 * @brief Interface for the fan PID controller built on top of the Arduino PID library.
 */
#pragma once
#include <PID_v1.h>

extern PID fanPID; ///< PID controller instance operating in REVERSE mode (higher temp → higher output)

/**
 * @brief Initialize the PID controller with the current temperature as the starting input.
 *
 * Sets the setpoint to targetTempF, configures output limits to [0, 100],
 * sets the sample time to 2000 ms, and enables AUTOMATIC mode.
 *
 * @param currentTempF Current measured temperature in degrees Fahrenheit used to seed pidInput.
 */
void initPID(float currentTempF);

/**
 * @brief Feed a new temperature reading into the PID and apply the computed output to the fan.
 *
 * Updates pidInput, calls PID::Compute(), then passes the result to setFanPercent().
 *
 * @param tempF Current temperature in degrees Fahrenheit.
 */
void updatePID(float tempF);
