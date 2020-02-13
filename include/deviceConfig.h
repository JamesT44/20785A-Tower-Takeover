#pragma once

#include "main.h"
#include "purePursuit.h"
#include "screenDisplay.h"

// Forward declarations
extern okapi::Controller mainController;
extern okapi::Motor tilterMtr, liftMtr;
extern std::shared_ptr<okapi::AsyncVelocityController<double, double>>
  tilterController;
extern std::shared_ptr<okapi::MotorGroup> chassisLMtrs, chassisRMtrs,
  intakeMtrs;
extern std::shared_ptr<okapi::ADIEncoder> LEnc, REnc, MEnc;
extern okapi::ChassisScales chassisScale, odomScale;
// extern std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> robotModel;
extern std::shared_ptr<okapi::OdomChassisController> chassisControl;
extern ScreenDisplay mainDisplay;
extern PurePursuit pursuit;
