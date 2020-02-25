#include "autonFuncs.h"
#include "basicFuncs.h"
#include "deviceConfig.h"

void deployRobot() {
  tilterMtr.moveAbsolute(4000, 100);
  setLift(1900);
  while (liftMtr.getPosition() < 1500) {
    pros::delay(10);
  }
  setIntake(-1);
  pros::delay(1000);
  setLift(0);
  setIntake(1);
  while (liftMtr.getPosition() > 300) {
    pros::delay(10);
  }
}

void intakeFiveCubes() {
  setIntake(1);
  robotModel->setMaxVoltage(7000);
  chassisControl->moveDistance(3.05_ft);
}
