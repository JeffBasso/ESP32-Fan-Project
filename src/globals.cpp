/**
 * @file globals.cpp
 * @brief Definitions and default values for all shared global variables.
 *
 * Initial values here are placeholders; loadPrefs() overwrites them at startup
 * with values stored in NVS.
 */
#include "globals.h"

float  targetTempF    = 0.0;  ///< loaded from prefs, default 75.0F
bool   runFullOnError = false; ///< loaded from prefs, default false
double Kp = 0.0, Ki = 0.0, Kd = 0.0; ///< loaded from prefs, defaults 2.0/0.1/0.0
double pidInput = 0.0, pidOutput = 0.0, pidSetpoint = 0.0;
