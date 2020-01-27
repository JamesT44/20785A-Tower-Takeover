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
  // auto choice = mainDisplay.getAutonSelection();
  // if (strcmp(choice.second, "5pt small goal") || true) {
  chassisControl->setMaxVelocity(80);
  setIntake(-1);
  pros::delay(500);
  // chassisControl->moveDistance(1.2_ft);
  liftMtr.moveAbsolute(300, 100);
  while (liftMtr.getPosition() < 290) {
    pros::delay(10);
  }
  // chassisControl->moveDistance(-0.7_ft);
  liftMtr.moveAbsolute(0, 100);
  setIntake(1);
  while (liftMtr.getPosition() > 10) {
    pros::delay(10);
  }
  chassisControl->moveDistance(4_ft);
  setIntake(0);
  chassisControl->moveDistance(-2.3_ft);
  chassisControl->turnAngle(135_deg);
  chassisControl->moveDistance(1.2_ft);
  setIntake(-0.5);
  pros::delay(500);
  setIntake(0);
  setTilterVelocity(1);
  while (tilterMtr.getPosition() < 2300) {
    pros::delay(10);
  }
  setTilterVelocity(0.5);
  while (tilterMtr.getPosition() < 5300) {
    pros::delay(10);
  }
  setTilterVelocity(0);
  chassisControl->moveDistance(-1.2_ft);
  tilterMtr.moveAbsolute(0, 100);
  // }
}
