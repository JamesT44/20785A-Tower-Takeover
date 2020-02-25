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
  // deployRobot();
  // tilterMtr.moveAbsolute(1800, 100);
  intakeFiveCubes();
  setIntake(0.75); // slow intake for swing turn
  chassisControl->turnAngle(-45_deg,
                            chassisControl->rightPivot); // swing turn
  robotModel->setMaxVoltage(12000);      // Fast drive again
  chassisControl->moveDistance(-0.5_ft); // Back from last cube
  chassisControl->turnAngle(-137_deg);   // Turn to face near wall
  chassisControl->moveDistance(1.5_ft);  // Drive to near goal
  chassisControl->turnAngle(-44_deg);    // Turn to goal
  setIntake(0);                          // Stop intake
  robotModel->forward(0.8);              // Realign
  setIntake(-0.5); // Outtake slightly whilst realigning
  pros::delay(200);
  setIntake(0);
  pros::delay(800);
  robotModel->stop();                     // End realign
  chassisControl->moveDistance(-0.15_ft); // Backout slightly
  tilterMtr.moveVoltage(12000);           // Fast tilt out portion
  while (tilterMtr.getPosition() < 7500) {
    pros::delay(10);
  }
  tilterMtr.moveVoltage(12000); // Slow tilt out portion
  while (tilterMtr.getPosition() < 9500) {
    pros::delay(10);
  }
  tilterMtr.moveVoltage(0);
  chassisControl->moveDistance(-1_ft); // Reverse out
}
