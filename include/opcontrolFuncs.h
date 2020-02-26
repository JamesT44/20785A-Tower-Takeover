#pragma once

#include "main.h"

// Some constants to use
const float tilterVelocity = 1;
const long tilterInPos = 0;
const long tilterOutPos = 1300;
const float liftPower = 1;
const float intakePower = 1;
const float slowIntakePower = 0.8;

const long liftPresets[] = {0, 1500, 1900};
const size_t liftPresetNum = sizeof(liftPresets) / sizeof(liftPresets[0]);
static size_t liftPresetIndex = 0;
static bool trayAutoIn = false;
static uint32_t prevAutoTime = 0;

void chassisOpcontrolTask(void *ignore);
void liftOpcontrol();
void tilterOpcontrol();
void intakeOpcontrol();
