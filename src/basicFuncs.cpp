#include "basicFuncs.h"
#include "deviceConfig.h"

// Basic wrappers to encapsulate logic and keep code DRY
void setChassisLeft(float power) {
  chassisLMtrs.moveVoltage(power * maxVoltage);
}

void setChassisRight(float power) {
  chassisRMtrs.moveVoltage(power * maxVoltage);
}

void setChassis(float leftPower, float rightPower) {
  setChassisLeft(leftPower);
  setChassisRight(rightPower);
}

void setChassis(float power) {
  setChassis(power, power);
}

void setTilter(float power) {
  tilterMtr.moveVoltage(power * maxVoltage);
}

void setIntake(float power) {
  intakeMtrs.moveVoltage(power * maxVoltage);
}
