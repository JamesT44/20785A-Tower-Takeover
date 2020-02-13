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

okapi::ControllerButton deployBtn(okapi::ControllerDigital::B);
okapi::ControllerButton odomTestBtn(okapi::ControllerDigital::Y);

void chassisOpcontrolTask(void *ignore) {
  std::uint32_t prev_time = pros::millis();
  while (true) {
    if (backwardBtn.isPressed()) {
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
      tilterMtr.moveAbsolute(1600, 200);
      while (tilterMtr.getPosition() < 1350) {
        pros::delay(10);
      }
      setIntake(-1);
      pros::delay(500);
      setLift(1900);
      while (liftMtr.getPosition() < 1800) {
        pros::delay(10);
      }
      tilterMtr.moveAbsolute(300, 200);
      while (tilterMtr.getPosition() > 600) {
        pros::delay(10);
      }
      setLift(0);
      setIntake(1);
    } else if (liftUpBtn.changedToReleased()) {
      if (liftPresetIndex < liftPresetNum - 1) {
        // tilterPrevPos = tilterMtr.getPosition();
        // if (liftPresetIndex == 0) {
        //   tilterMtr.moveAbsolute(tilterOutPos, 100);
        // }
        liftPresetIndex += 1;
        setLift(liftPresets[liftPresetIndex]);
      }
    } else if (liftDownBtn.changedToReleased()) {
      if (liftPresetIndex > 0) {
        liftPresetIndex -= 1;
        // if (liftPresetIndex == 0) {
        //   tilterMtr.moveAbsolute(tilterPrevPos, 100);
        // }
        setLift(liftPresets[liftPresetIndex]);
      }
    }

    pros::delay(10);
  }
}

void tilterOpcontrol() {
  if (tiltOutBtn.isPressed()) {
    liftManual = true;
    if (tilterMtr.getPosition() > 2000) {
      setTilterVelocity(tilterOutSlowVelocity);
    } else {
      setTilterVelocity(tilterOutFastVelocity);
    }
  } else if (tiltInBtn.isPressed()) {
    liftManual = true;
    setTilterVelocity(-tilterInVelocity);
  } else if (liftManual) {
    setTilterVelocity(0);
    liftManual = false;
  }
}

void intakeOpcontrol() {
  if (intakeBtn.isPressed()) {
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
    pros::delay(1500);
  }
}
