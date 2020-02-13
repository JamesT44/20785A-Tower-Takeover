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
  bool blue = false;
  chassisControl->setMaxVelocity(130);
  // chassisControl->moveDistanceAsync(1_ft);
  // chassisControl->waitUntilSettled();
  // chassisControl->moveDistance(-1_ft);
  // chassisControl->waitUntilSettled();
  tilterMtr.moveAbsolute(1600, 200);
  while (tilterMtr.getPosition() < 1350) {
    pros::delay(10);
  }
  setIntake(-1);
  pros::delay(500);
  setLift(1900);
  while (liftMtr.getPosition() < 1800) {
    pros::delay(10);
  }
  tilterMtr.moveAbsolute(300, 200);
  while (tilterMtr.getPosition() > 600) {
    pros::delay(10);
  }
  setLift(0);
  setIntake(1);
  pros::delay(500);
  setIntake(0.75);
  chassisControl->setMaxVelocity(80);
  chassisControl->moveDistance(3.5_ft);
  chassisControl->setMaxVelocity(120);
  chassisControl->moveDistance(-1.60_ft);
  chassisControl->turnAngle((blue ? -1 : 1) * 135_deg);
  setChassis(0.75);
  pros::delay(1000);
  setChassis(0);
  setIntake(0);
  chassisControl->moveDistance(-0.15_ft);
  setTilterVelocity(1);
  while (tilterMtr.getPosition() < 2000) {
    pros::delay(10);
  }
  setTilterVelocity(0.4);
  while (tilterMtr.getPosition() < 4500) {
    pros::delay(10);
  }
  setTilterVelocity(0);
  chassisControl->moveDistance(-1.35_ft);
  tilterMtr.moveAbsolute(0, 100);
}
