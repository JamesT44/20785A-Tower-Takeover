#include "basicFuncs.h"
#include "deviceConfig.h"

void setChassis(double leftPower, double rightPower) {
  robotModel->tank(leftPower, rightPower);
}

void setChassis(double power) {
  setChassis(power, power);
}

void setLift(long encoderTicks) {
  liftMtr.moveAbsolute(encoderTicks, 80);
}

void setTilter(double power) {
}

void setTilterVelocity(double velocity) {
  tilterController->setTarget(velocity * 100);
}

void setIntake(double power) {
  intakeMtrs->moveVoltage(power * maxVoltage);
}
