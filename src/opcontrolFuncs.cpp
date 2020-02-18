#include "opcontrolFuncs.h"
#include "basicFuncs.h"
#include "deviceConfig.h"
#include "purePursuit.h"

// Named buttons
okapi::ControllerButton tiltOutBtn(okapi::ControllerDigital::up);
okapi::ControllerButton tiltInBtn(okapi::ControllerDigital::down);
okapi::ControllerButton backwardBtn(okapi::ControllerDigital::left);
okapi::ControllerButton backOuttakeBtn(okapi::ControllerDigital::right);

okapi::ControllerButton liftUpBtn(okapi::ControllerDigital::L1);
okapi::ControllerButton liftDownBtn(okapi::ControllerDigital::L2);
okapi::ControllerButton intakeBtn(okapi::ControllerDigital::R1);
okapi::ControllerButton outtakeBtn(okapi::ControllerDigital::R2);

okapi::ControllerButton deployBtn(okapi::ControllerDigital::B);
okapi::ControllerButton odomTestBtn(okapi::ControllerDigital::Y);

void chassisOpcontrolTask(void *ignore) {
  std::uint32_t prev_time = pros::millis();
  while (true) {
    if (backwardBtn.isPressed()) {
      setChassis(-0.25);
    } else if (backOuttakeBtn.isPressed()) {
      setChassis(-0.25);
    } else {
      setChassis(
        mainController.getAnalog(okapi::ControllerAnalog::leftY),
        mainController.getAnalog(okapi::ControllerAnalog::rightY));
    }

    pros::delay(10);
  }
}

void liftOpcontrolTask(void *ignore) {
  setLift(liftPresets[liftPresetIndex]);
  while (true) {
    if (deployBtn.isPressed()) {
      tilterMtr.moveAbsolute(3250, 100);
      while (tilterMtr.getPosition() < 3000) {
        pros::delay(10);
      }
      setLift(1900);
      tilterMtr.moveAbsolute(1500, 100);
      while (tilterMtr.getPosition() > 1750) {
        pros::delay(10);
      }
      while (liftMtr.getPosition() < 1600) {
        pros::delay(10);
      }
      setIntake(-1);
      pros::delay(1000);
      setLift(0);
    } else if (liftUpBtn.changedToReleased()) {
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
    if (tilterMtr.getPosition() > 7000) {
      setTilterVelocity(0.75);
    } else {
      setTilterVelocity(tilterVelocity);
    }
  } else if (tiltInBtn.isPressed()) {
    setTilterVelocity(-tilterVelocity);
  } else {
    setTilterVelocity(0);
  }
}

void intakeOpcontrol() {
  if (backOuttakeBtn.isPressed()) {
    setIntake(-0.5);
  } else if (intakeBtn.isPressed()) {
    setIntake(intakePower);
  } else if (outtakeBtn.isPressed()) {
    if (liftPresetIndex == 1) {
      setIntake(-slowIntakePower);
    } else {
      setIntake(-intakePower);
    }
  } else {
    setIntake(0);
  }
  if (deployBtn.isPressed()) {
    pros::delay(3000);
  }
}
