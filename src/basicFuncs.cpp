#include "basicFuncs.h"
#include "deviceConfig.h"

void setChassis(double leftPower, double rightPower) {
  robotModel->tank(leftPower, rightPower);
}

void setChassis(double power) {
  setChassis(power, power);
}

void setTilter(double power) {
  tilterMtr.moveVoltage(power * maxVoltage);
}

void setIntake(double power) {
  intakeMtrs->moveVoltage(power * maxVoltage);
}
