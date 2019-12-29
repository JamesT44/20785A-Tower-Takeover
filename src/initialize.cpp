#include "main.h"

okapi::Controller mainController(okapi::ControllerId::master);

okapi::Motor chassisFLMtr = okapi::Motor(1, false, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor chassisFRMtr = okapi::Motor(3, true, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor chassisBLMtr = okapi::Motor(2, false, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor chassisBRMtr = okapi::Motor(4, true, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor tilterMtr = okapi::Motor(5, false, okapi::AbstractMotor::gearset::red, okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor intakeLMtr = okapi::Motor(6, false, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor intakeRMtr = okapi::Motor(7, true, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::counts);

okapi::MotorGroup chassisLMtrs = okapi::MotorGroup({chassisFLMtr, chassisBLMtr});
okapi::MotorGroup chassisRMtrs = okapi::MotorGroup({chassisFRMtr, chassisBRMtr});
okapi::MotorGroup intakeMtrs = okapi::MotorGroup({intakeLMtr, intakeRMtr});

okapi::ADIEncoder LEnc = okapi::ADIEncoder(7, 8);
okapi::ADIEncoder REnc = okapi::ADIEncoder(1, 2, true);
okapi::ADIEncoder MEnc = okapi::ADIEncoder(3, 4);

okapi::ChassisScales chassisScale = okapi::ChassisScales({4.157_in, 11.15_in}, 900);
okapi::ChassisScales odomScale = okapi::ChassisScales({2.8193_in, 4.549_in, 4.5_in}, 360);

std::shared_ptr<okapi::OdomChassisController> chassisControl = okapi::ChassisControllerBuilder()
		.withMotors(chassisLMtrs, chassisRMtrs)
		.withDimensions(okapi::AbstractMotor::gearset::green, chassisScale)
		.withSensors(LEnc, REnc, MEnc)
		.withMaxVelocity(70)
		.withOdometry(odomScale, okapi::StateMode::CARTESIAN)
		.buildOdometry();

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
