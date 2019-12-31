#include "customOdometry.h"
#include "debugScreen.h"
#include "main.h"

okapi::Controller mainController(okapi::ControllerId::master);

okapi::Motor tilterMtr = okapi::Motor(5,
                                      false,
                                      okapi::AbstractMotor::gearset::red,
                                      okapi::AbstractMotor::encoderUnits::counts);

std::shared_ptr<okapi::MotorGroup> chassisLMtrs =
  std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({1, 2}));
std::shared_ptr<okapi::MotorGroup> chassisRMtrs =
  std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({-3, -4}));
std::shared_ptr<okapi::MotorGroup> intakeMtrs =
  std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({6, -7}));

std::shared_ptr<okapi::ADIEncoder> LEnc = std::make_shared<okapi::ADIEncoder>(7, 8);
std::shared_ptr<okapi::ADIEncoder> REnc = std::make_shared<okapi::ADIEncoder>(1, 2, true);
std::shared_ptr<okapi::ADIEncoder> MEnc = std::make_shared<okapi::ADIEncoder>(3, 4);

okapi::ChassisScales chassisScale = okapi::ChassisScales({4.157_in, 11.15_in}, 900);
okapi::ChassisScales odomScale = okapi::ChassisScales({2.8193_in, 4.549_in, 4.5_in}, 360);

std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> robotModel =
  std::make_shared<okapi::ThreeEncoderSkidSteerModel>(chassisLMtrs,
                                                      chassisRMtrs,
                                                      LEnc,
                                                      REnc,
                                                      MEnc,
                                                      200,
                                                      12000);

std::unique_ptr<CustomOdometry> robotOdometry =
  std::make_unique<CustomOdometry>(okapi::TimeUtilFactory().create(), robotModel, odomScale);

std::shared_ptr<okapi::OdomChassisController> chassisControl =
  okapi::ChassisControllerBuilder()
    .withMotors(chassisLMtrs, chassisRMtrs)
    .withDimensions(okapi::AbstractMotor::gearset::green, chassisScale)
    .withSensors(LEnc, REnc, MEnc)
    .withMaxVelocity(70)
    .withOdometry(std::move(robotOdometry), okapi::StateMode::CARTESIAN)
    .buildOdometry();

DebugScreen debugDisplay(lv_scr_act(), chassisControl);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
}
