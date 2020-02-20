#include "deviceConfig.h"
#include "lib7842/api.hpp"
#include "main.h"

using namespace lib7842;

okapi::Controller mainController(okapi::ControllerId::master);

okapi::Motor tilterMtr =
  okapi::Motor(5,
               false,
               okapi::AbstractMotor::gearset::red,
               okapi::AbstractMotor::encoderUnits::counts);

okapi::Motor liftMtr =
  okapi::Motor(9,
               true,
               okapi::AbstractMotor::gearset::red,
               okapi::AbstractMotor::encoderUnits::counts);

std::shared_ptr<okapi::MotorGroup> chassisLMtrs =
  std::make_shared<okapi::MotorGroup>(
    std::initializer_list<okapi::Motor>({1, 2}));
std::shared_ptr<okapi::MotorGroup> chassisRMtrs =
  std::make_shared<okapi::MotorGroup>(
    std::initializer_list<okapi::Motor>({-8, -4}));
std::shared_ptr<okapi::MotorGroup> intakeMtrs =
  std::make_shared<okapi::MotorGroup>(
    std::initializer_list<okapi::Motor>({6, -7}));

// std::shared_ptr<okapi::ADIEncoder> LEnc =
//   std::make_shared<okapi::ADIEncoder>(7, 8);
// std::shared_ptr<okapi::ADIEncoder> REnc =
//   std::make_shared<okapi::ADIEncoder>(1, 2, true);
// std::shared_ptr<okapi::ADIEncoder> MEnc =
//   std::make_shared<okapi::ADIEncoder>(3, 4);

okapi::ChassisScales chassisScale =
  okapi::ChassisScales({4.157_in, 11.15_in}, okapi::imev5GreenTPR);
okapi::ChassisScales odomScale =
  okapi::ChassisScales({2.8193_in, 4.549_in, 4.5_in},
                       okapi::quadEncoderTPR);

std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> robotModel =
  std::make_shared<okapi::ThreeEncoderSkidSteerModel>(
    chassisLMtrs,
    chassisRMtrs,
    std::make_shared<ADIEncoder>(7, 8),
    std::make_shared<ADIEncoder>(1, 2, true),
    std::make_shared<ADIEncoder>(3, 4),
    200,
    12000);

std::shared_ptr<CustomOdometry> robotOdometry =
  std::make_shared<CustomOdometry>(robotModel,
                                   odomScale,
                                   TimeUtilFactory().create());

std::shared_ptr<OdomController> chassisControl =
  std::make_shared<OdomController>(
    robotModel,
    robotOdometry,
    // Distance PID - To mm
    std::make_unique<IterativePosPIDController>(
      0.012,
      0.0000,
      0.0002,
      0,
      TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
    // Turn PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.1,
      0.00,
      0.004,
      0,
      TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)),
    // Angle PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.02,
      0,
      0,
      0,
      TimeUtilFactory::withSettledUtilParams(4, 2, 100_ms)),
    1_ft,
    TimeUtilFactory().create());

PathFollower follower(robotModel,
                      robotOdometry,
                      chassisScale,
                      1_ft,
                      TimeUtilFactory().create());
PursuitLimits limits{0.2_mps, 1.1_mps2, 0.75_mps, 1.1_mps2, 0_mps, 40_mps};

ScreenDisplay mainDisplay(lv_scr_act(), robotOdometry);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  robotOdometry->startTask("Odometry");
  intakeMtrs->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  tilterMtr.tarePosition();
  liftMtr.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  liftMtr.tarePosition();
}

/**
 * Runs while the robot is in the disabled state of Field Management System
 * or the VEX Competition Switch, following either autonomous or opcontrol.
 * When the robot is enabled, this task will exit.
 */
void disabled() {
}

/**
 * Runs after initialize(), and before autonomous when connected to the
 * Field Management System or the VEX Competition Switch. This is intended
 * for competition-specific initialization routines, such as an autonomous
 * selector on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or
 * opcontrol starts.
 */
void competition_initialize() {
}
