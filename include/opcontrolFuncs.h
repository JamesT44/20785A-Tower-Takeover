#pragma once

#include "main.h"

// Some constant powers to use
<<<<<<< Updated upstream
const float tilterOutVelocity = 0.4;
const float tilterInVelocity = 0.6;
=======
const float tilterOutPower = 1;
const float tilterInPower = 0.5;
>>>>>>> Stashed changes
const float liftPower = 1;
const float intakePower = 1;

void chassisOpcontrolTask(void *ignore);
void tilterOpcontrol();
void intakeOpcontrol();
