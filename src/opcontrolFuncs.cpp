#include "opcontrolFuncs.h"
#include "basicFuncs.h"
#include "deviceConfig.h"
#include "purePursuit.h"

// Named buttons
okapi::ControllerButton tiltOutBtn(okapi::ControllerDigital::up);
okapi::ControllerButton tiltInBtn(okapi::ControllerDigital::down);
okapi::ControllerButton backwardBtn(okapi::ControllerDigital::left);

okapi::ControllerButton liftUpBtn(okapi::ControllerDigital::L1);
okapi::ControllerButton liftDownBtn(okapi::ControllerDigital::L2);
okapi::ControllerButton intakeBtn(okapi::ControllerDigital::R1);
okapi::ControllerButton outtakeBtn(okapi::ControllerDigital::R2);

okapi::ControllerButton pursuitTestBtn(okapi::ControllerDigital::X);
okapi::ControllerButton odomTestBtn(okapi::ControllerDigital::Y);

void chassisOpcontrolTask(void *ignore) {
  std::uint32_t prev_time = pros::millis();
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

void liftOpcontrolTask(void *ignore) {
  size_t liftPresetIndex = 0;
  setLift(liftPresets[liftPresetIndex]);
  while (true) {
    if (liftUpBtn.changedToReleased()) {
      if (liftPresetIndex < liftPresetNum - 1) {
        liftPresetIndex += 1;
        setLift(liftPresets[liftPresetIndex]);
      }
    } else if (liftDownBtn.changedToReleased()) {
      if (liftPresetIndex > 0) {
        liftPresetIndex -= 1;
        setLift(liftPresets[liftPresetIndex]);
      }
    }

    pros::delay(10);
  }
}

void tilterOpcontrol() {
  if (tiltOutBtn.isPressed()) {
    if (tilterMtr.getPosition() > 2300) {
      setTilterVelocity(tilterOutSlowVelocity);
    } else {
      setTilterVelocity(tilterOutFastVelocity);
    }
  } else if (tiltInBtn.isPressed()) {
    setTilterVelocity(-tilterInVelocity);
  } else {
    setTilterVelocity(0);
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
