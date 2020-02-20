#pragma once

#include "lib7842/api.hpp"
#include "main.h"
#include "screenDisplay.h"

using namespace lib7842;

// Forward declarations
extern okapi::Controller mainController;
extern okapi::Motor tilterMtr, liftMtr;
extern std::shared_ptr<okapi::MotorGroup> chassisLMtrs, chassisRMtrs,
  intakeMtrs;
// extern std::shared_ptr<okapi::ADIEncoder> LEnc, REnc, MEnc;
extern okapi::ChassisScales chassisScale, odomScale;
extern std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> robotModel;
extern std::shared_ptr<CustomOdometry> robotOdometry;
extern std::shared_ptr<OdomController> chassisControl;
extern ScreenDisplay mainDisplay;
extern PathFollower follower;
extern PursuitLimits limits;
