#include "customOdometry.h"
#include "debugScreen.h"
#include "main.h"
#include "purePursuit.h"

okapi::Controller mainController(okapi::ControllerId::master);

okapi::Motor tilterMtr = okapi::Motor(5,
                                      false,
                                      okapi::AbstractMotor::gearset::red,
                                      okapi::AbstractMotor::encoderUnits::counts);

std::shared_ptr<okapi::MotorGroup> chassisLMtrs =
  std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({1, 2}));
std::shared_ptr<okapi::MotorGroup> chassisRMtrs =
  std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({-8, -4}));
std::shared_ptr<okapi::MotorGroup> intakeMtrs =
  std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({6, -7}));

std::shared_ptr<okapi::ADIEncoder> LEnc = std::make_shared<okapi::ADIEncoder>(7, 8);
std::shared_ptr<okapi::ADIEncoder> REnc = std::make_shared<okapi::ADIEncoder>(1, 2, true);
std::shared_ptr<okapi::ADIEncoder> MEnc = std::make_shared<okapi::ADIEncoder>(3, 4);

okapi::ChassisScales chassisScale =
  okapi::ChassisScales({4.157_in, 11.15_in}, okapi::imev5GreenTPR);
okapi::ChassisScales odomScale =
  okapi::ChassisScales({2.8193_in, 4.549_in, 4.5_in}, okapi::quadEncoderTPR);

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

auto cci = std::make_shared<okapi::ChassisControllerIntegrated>(
  okapi::TimeUtilFactory().create(),
  robotModel,
  std::make_unique<okapi::AsyncPosIntegratedController>(
    chassisLMtrs,
    okapi::AbstractMotor::gearset::green,
    okapi::toUnderlyingType(okapi::AbstractMotor::gearset::green),
    okapi::TimeUtilFactory().create(),
    okapi::Logger::getDefaultLogger()),
  std::make_unique<okapi::AsyncPosIntegratedController>(
    chassisRMtrs,
    okapi::AbstractMotor::gearset::green,
    okapi::toUnderlyingType(okapi::AbstractMotor::gearset::green),
    okapi::TimeUtilFactory().create(),
    okapi::Logger::getDefaultLogger()),
  okapi::AbstractMotor::gearset::green,
  chassisScale);

std::shared_ptr<okapi::OdomChassisController> chassisControl =
  std::make_shared<okapi::DefaultOdomChassisController>(okapi::TimeUtilFactory().create(),
                                                        std::move(robotOdometry),
                                                        cci);

DebugScreen debugDisplay(lv_scr_act(), chassisControl);

PurePursuit pursuit = PurePursuit(chassisControl, 1.1_ft);
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  chassisControl->startOdomThread();
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
