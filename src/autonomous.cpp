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
  bool blue = true;
  chassisControl->setMaxVelocity(130);
  tilterMtr.moveAbsolute(3250, 100);
  while (tilterMtr.getPosition() < 3000) {
    pros::delay(10);
  }
  setLift(1900);
  tilterMtr.moveAbsolute(1500, 100);
  while (tilterMtr.getPosition() > 1750) {
    pros::delay(10);
  }
  while (liftMtr.getPosition() < 1600) {
    pros::delay(10);
  }
  setIntake(-1);
  pros::delay(1000);
  setLift(0);
  setIntake(1);
  pros::delay(1000);
  chassisControl->setMaxVelocity(110);
  chassisControl->moveDistance(3.25_ft);
  chassisControl->setMaxVelocity(200);
  if (blue) {
    chassisControl->turnAngle((blue ? -1 : 1) * -33_deg);
  } else {
    chassisControl->turnAngle((blue ? -1 : 1) * -35_deg);
  }
  chassisControl->moveDistance(0.5_ft);
  chassisControl->moveDistance(-1.5_ft);
  if (blue) {
    chassisControl->turnAngle((blue ? -1 : 1) * -180_deg);
  } else {
    chassisControl->turnAngle((blue ? -1 : 1) * -171_deg);
  }
  setIntake(0);
  // chassisControl->moveDistanceAsync(blue ? 0.9_ft : 0.97_ft);
  setChassis(0.8);
  setIntake(-0.5);
  pros::delay(200);
  setIntake(0);
  pros::delay(550);
  setChassis(0);
  setIntake(0);
  tilterMtr.moveVoltage(12000);
  while (tilterMtr.getPosition() < 7500) {
    pros::delay(10);
  }
  tilterMtr.moveVoltage(10000);
  while (tilterMtr.getPosition() < 9500) {
    pros::delay(10);
  }
  tilterMtr.moveVoltage(0);
  chassisControl->moveDistance(-1.35_ft);
  tilterMtr.moveAbsolute(0, 100);
  // Practice interview
  // Remove drive back
  // Change settledutil parameters
}
