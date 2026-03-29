/**
 * @file pid_control.cpp
 * @brief PID controller implementation for closed-loop fan speed regulation.
 */
#include "pid_control.h"
#include "config.h"
#include "globals.h"
#include "fan.h"

PID fanPID(&pidInput, &pidOutput, &pidSetpoint, Kp, Ki, Kd, REVERSE);

void initPID(float currentTempF) {
  pidSetpoint = targetTempF;
  pidInput    = currentTempF;
  fanPID.SetOutputLimits(0, 100);
  fanPID.SetSampleTime(CONTROL_INTERVAL_MS);
  fanPID.SetMode(AUTOMATIC);
}

void updatePID(float tempF) {
  pidInput = tempF;
  fanPID.Compute();
  setFanPercent((int)pidOutput);
}
