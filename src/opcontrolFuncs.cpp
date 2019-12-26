#include "opcontrolFuncs.h"
#include "deviceConfig.h"
#include "basicFuncs.h"

// Named buttons
okapi::ControllerButton forwardBtn(okapi::ControllerDigital::up);
okapi::ControllerButton backwardBtn(okapi::ControllerDigital::down);
okapi::ControllerButton turnLBtn(okapi::ControllerDigital::left);
okapi::ControllerButton turnRBtn(okapi::ControllerDigital::right);

okapi::ControllerButton tiltOutBtn(okapi::ControllerDigital::L1);
okapi::ControllerButton tiltInBtn(okapi::ControllerDigital::L2);
okapi::ControllerButton intakeBtn(okapi::ControllerDigital::R1);
okapi::ControllerButton outtakeBtn(okapi::ControllerDigital::R2);

void chassisOpcontrol() {
  if (forwardBtn.isPressed()) {
    setChassis(0.5);
  } else if (backwardBtn.isPressed()) {
    setChassis(-0.5);
  } else if (turnLBtn.isPressed()) {
    setChassis(0.5, -0.5);
  } else if (turnRBtn.isPressed()) {
    setChassis(-0.5, 0.5);
  } else {
    setChassis(mainController.getAnalog(okapi::ControllerAnalog::leftY),
              mainController.getAnalog(okapi::ControllerAnalog::rightY));
  }
}

void tilterOpcontrol() {
  if (tiltOutBtn.isPressed()) {
    setTilter(tilterOutPower);
  } else if (tiltInBtn.isPressed()) {
    setTilter(-tilterInPower);
  } else {
    setTilter(0);
  }
}

void intakeOpcontrol() {
  if (intakeBtn.isPressed()) {
    setIntake(intakePower);
  } else if (outtakeBtn.isPressed()) {
    setIntake(-intakePower);
  } else {
    setIntake(0);
  }
}
