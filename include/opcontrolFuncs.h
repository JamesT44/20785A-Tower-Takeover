#pragma once

#include "main.h"

// Some constant powers to use
const float tilterOutSlowVelocity = 0.4;
const float tilterOutFastVelocity = 0.7;
const float tilterInVelocity = 0.75;
const float tilterOutPower = 1;
const float tilterInPower = 0.5;
const float liftPower = 1;
const float intakePower = 1;

const long liftPresets[] = {0, 1500, 1900};
const size_t liftPresetNum = sizeof(liftPresets) / sizeof(liftPresets[0]);

void chassisOpcontrolTask(void *ignore);
void liftOpcontrolTask(void *ignore);
void tilterOpcontrol();
void intakeOpcontrol();
