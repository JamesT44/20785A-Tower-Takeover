#pragma once

#include "main.h"

const int maxVoltage = 12000;

void setChassisLeft(float power);
void setChassisRight(float power);

// Overloaded for backwards compatibility
void setChassis(float leftPower, float rightPower);
void setChassis(float power);

void setTilter(float power);
void setIntake(float power);
