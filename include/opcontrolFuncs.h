#pragma once

#include "main.h"

// Some constant powers to use
const float tilterOutPower = 1;
const float tilterInPower = 0.5;
const float liftPower = 1;
const float intakePower = 1;

const float liftPresets[] = {0, 20, 40};
static int liftPresetIndex = 0;
const size_t numLiftPresets = sizeof(liftPresets) / sizeof(liftPresets[0]);

void chassisOpcontrolTask(void *ignore);
void liftOpcontrol();
void tilterOpcontrol();
void intakeOpcontrol();
