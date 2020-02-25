#include "opcontrolFuncs.h"
#include "autonFuncs.h"
#include "basicFuncs.h"
#include "deviceConfig.h"

// Named buttons
okapi::ControllerButton tiltOutBtn(okapi::ControllerDigital::up);
okapi::ControllerButton tiltInBtn(okapi::ControllerDigital::down);
okapi::ControllerButton tiltAutoInBtn(okapi::ControllerDigital::X);
okapi::ControllerButton backwardBtn(okapi::ControllerDigital::left);
okapi::ControllerButton backOuttakeBtn(okapi::ControllerDigital::right);

okapi::ControllerButton liftUpBtn(okapi::ControllerDigital::L1);
okapi::ControllerButton liftDownBtn(okapi::ControllerDigital::L2);
okapi::ControllerButton intakeBtn(okapi::ControllerDigital::R1);
okapi::ControllerButton outtakeBtn(okapi::ControllerDigital::R2);

okapi::ControllerButton deployBtn(okapi::ControllerDigital::B);
okapi::ControllerButton odomBwdBtn(okapi::ControllerDigital::Y);

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

    if (odomBwdBtn.changedToPressed()) {
      chassisControl->turnAngle(180_deg);
      while (!chassisControl->isTurnSettled()) {
        pros::delay(10);
      }
    }

    pros::delay(10);
  }
}

void liftOpcontrol() {
  if (deployBtn.isPressed()) {
    deployRobot(true);
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
}

void tilterOpcontrol() {
  if (deployBtn.changedToReleased()) {
    trayAutoIn = true;
    tilterMtr.moveAbsolute(1800, 100);
  }

  if (tiltOutBtn.isPressed()) {
    if (tilterMtr.getPosition() > 6000) {
      setTilterVelocity(0.75);
    } else {
      setTilterVelocity(tilterVelocity);
    }
    trayAutoIn = false;
  } else if (tiltInBtn.isPressed()) {
    setTilterVelocity(-tilterVelocity);
    trayAutoIn = false;
  } else if (tiltAutoInBtn.changedToReleased()) {
    trayAutoIn = true;
    tilterMtr.moveAbsolute(1800, 100);
  } else if (!trayAutoIn) {
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
