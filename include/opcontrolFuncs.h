#pragma once

#include "main.h"

// Some constant powers to use
const float tilterOutVelocity = 0.4;
const float tilterInVelocity = 0.6;
const float liftPower = 1;
const float intakePower = 1;

void chassisOpcontrolTask(void *ignore);
void tilterOpcontrol();
void intakeOpcontrol();
