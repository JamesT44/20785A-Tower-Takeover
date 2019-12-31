#pragma once

#include "main.h"
#include "debugScreen.h"
#include "customOdometry.h"

// Forward declarations
extern okapi::Controller mainController;
extern okapi::Motor tilterMtr;
extern std::shared_ptr<okapi::MotorGroup> chassisLMtrs, chassisRMtrs, intakeMtrs;
extern std::shared_ptr<okapi::ADIEncoder> LEnc, REnc, MEnc;
extern okapi::ChassisScales chassisScale, odomScale;
extern std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> robotModel;
extern std::shared_ptr<okapi::OdomChassisController> chassisControl;
extern DebugScreen debugDisplay;
