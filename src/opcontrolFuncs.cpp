#include "opcontrolFuncs.h"
#include "basicFuncs.h"
#include "deviceConfig.h"
#include "purePursuit.h"

// Named buttons
okapi::ControllerButton forwardBtn(okapi::ControllerDigital::up);
okapi::ControllerButton backwardBtn(okapi::ControllerDigital::down);
okapi::ControllerButton turnLBtn(okapi::ControllerDigital::left);
okapi::ControllerButton turnRBtn(okapi::ControllerDigital::right);

okapi::ControllerButton tiltOutBtn(okapi::ControllerDigital::L1);
okapi::ControllerButton tiltInBtn(okapi::ControllerDigital::L2);
okapi::ControllerButton intakeBtn(okapi::ControllerDigital::R1);
okapi::ControllerButton outtakeBtn(okapi::ControllerDigital::R2);

okapi::ControllerButton pursuitTestBtn(okapi::ControllerDigital::X);
okapi::ControllerButton odomTestBtn(okapi::ControllerDigital::Y);

void chassisOpcontrolTask(void *ignore) {
  while (true) {
    if (backwardBtn.isPressed()) {
      setChassis(-0.25);
    } else {
      setChassis(mainController.getAnalog(okapi::ControllerAnalog::leftY),
                 mainController.getAnalog(okapi::ControllerAnalog::rightY));
    }

    pros::delay(10);
  }
}

void tilterOpcontrol() {
  if (tiltOutBtn.isPressed()) {
    setTilterVelocity(tilterOutVelocity);
  } else if (tiltInBtn.isPressed()) {
    setTilterVelocity(-tilterInVelocity);
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
