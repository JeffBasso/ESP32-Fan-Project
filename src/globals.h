/**
 * @file globals.h
 * @brief Extern declarations for shared mutable state defined in globals.cpp.
 *
 * All modules that need to read or write these variables include this header.
 */
#pragma once

extern float  targetTempF;     ///< Desired temperature setpoint in degrees Fahrenheit
extern bool   runFullOnError;  ///< When true, run fan at 100% if the temperature probe is lost
extern double Kp;              ///< PID proportional gain
extern double Ki;              ///< PID integral gain
extern double Kd;              ///< PID derivative gain
extern double pidInput;        ///< PID input: current temperature in degrees Fahrenheit
extern double pidOutput;       ///< PID output: fan duty cycle percentage (0–100)
extern double pidSetpoint;     ///< PID setpoint: mirrors targetTempF in degrees Fahrenheit
