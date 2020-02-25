#include "autonFuncs.h"
#include "basicFuncs.h"
#include "deviceConfig.h"
#include "main.h"

/**
 * Runs the user autonomous code. This function will be started in its own
 * task with the default priority and stack size whenever the robot is
 * enabled via the Field Management System or the VEX Competition Switch in
 * the autonomous mode. Alternatively, this function may be called in
 * initialize or opcontrol for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not
 * re-start it from where it left off.
 */
void autonomous() {
  deployRobot(true);
  intakeFiveCubes();
  setIntake(0.75);
  chassisControl->turnAngle(-45_deg, chassisControl->rightPivot);
  robotModel->setMaxVoltage(12000);
  chassisControl->moveDistance(-0.5_ft);
  chassisControl->turnAngle(-137_deg);
  chassisControl->moveDistance(1.5_ft);
  chassisControl->turnAngle(-47_deg);
  setIntake(0);
  robotModel->forward(0.8);
  setIntake(-0.5);
  pros::delay(200);
  setIntake(0);
  pros::delay(800);
  robotModel->stop();
  chassisControl->moveDistance(-0.15_ft);
  tilterMtr.moveVoltage(12000);
  while (tilterMtr.getPosition() < 7500) {
    pros::delay(10);
  }
  tilterMtr.moveVoltage(12000);
  while (tilterMtr.getPosition() < 9500) {
    pros::delay(10);
  }
  tilterMtr.moveVoltage(0);
  chassisControl->moveDistance(-1_ft);
}
