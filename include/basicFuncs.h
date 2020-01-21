#pragma once

#include "main.h"

const int maxVoltage = 12000;

// Overloaded for backwards compatibility
void setChassis(double leftPower, double rightPower);
void setChassis(double power);

void setTilter(double power);
void setTilterVelocity(double velocity);

void setIntake(double power);
