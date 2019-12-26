#pragma once

#include "main.h"

// Forward declarations
extern okapi::Controller mainController;
extern okapi::Motor chassisFLMtr, chassisFRMtr, chassisBLMtr, chassisBRMtr,
                    tilterMtr, intakeLMtr, intakeRMtr;
extern okapi::MotorGroup chassisLMtrs, chassisRMtrs, intakeMtrs;
extern okapi::ADIEncoder LEnc, REnc, MEnc;
extern okapi::ChassisScales chassisScale, odomScale;
extern std::shared_ptr<okapi::OdomChassisController> chassisControl;
