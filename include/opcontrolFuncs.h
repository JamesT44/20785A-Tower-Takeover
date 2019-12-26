#pragma once

#include "main.h"

// Some constant powers to use
const float tilterOutPower = 0.5;
const float tilterInPower = 0.5;
const float liftPower = 1;
const float intakePower = 1;

void chassisOpcontrol();
void tilterOpcontrol();
void intakeOpcontrol();
