#include "opcontrolFuncs.h"
#include "basicFuncs.h"
#include "deviceConfig.h"
#include "purePursuit.h"

// Named buttons
okapi::ControllerButton tiltOutBtn(okapi::ControllerDigital::up);
okapi::ControllerButton tiltInBtn(okapi::ControllerDigital::down);
okapi::ControllerButton intakeBtn(okapi::ControllerDigital::R1);
okapi::ControllerButton outtakeBtn(okapi::ControllerDigital::R2);
okapi::ControllerButton liftUpBtn(okapi::ControllerDigital::L1);
okapi::ControllerButton liftDownBtn(okapi::ControllerDigital::L2);

okapi::ControllerButton pursuitTestBtn(okapi::ControllerDigital::X);
okapi::ControllerButton odomTestBtn(okapi::ControllerDigital::Y);

void chassisOpcontrolTask(void *ignore) {
  std::uint32_t prev_time = pros::millis();
  while (true) {
    if (pursuitTestBtn.changedToReleased()) {

      auto path = Trajectory2D({{0_ft, 0_ft}, {0_ft, 4_ft}, {2_ft, 4_ft}, {4_ft, 2_ft}})
                    .withInterpolation(1_cm)
                    .withSmoothening(0.9, 1000)
                    .generateTrajectory(0.75_mps, 0.4_mps2, 3_Hz);

      chassisControl->setState({0_ft, 0_ft, 0_deg});
      pursuit.executeTrajectory(path, 1.1_mps2);
    } else if (odomTestBtn.changedToReleased()) {
      chassisControl->setState({0_ft, 0_ft, 0_deg});
      chassisControl->driveToPoint({0_ft, 4_ft});
      chassisControl->driveToPoint({4_ft, 4_ft});
      chassisControl->driveToPoint({4_ft, 0_ft});
      chassisControl->driveToPoint({0_ft, 0_ft});
    } else {
      setChassis(mainController.getAnalog(okapi::ControllerAnalog::leftY),
                 mainController.getAnalog(okapi::ControllerAnalog::rightY));
    }

    pros::Task::delay_until(&prev_time, 10);
  }
}

void liftOpcontrol() {
  if (liftUpBtn.changedToReleased()) {
    if (liftPresetIndex != numLiftPresets - 1) {
      liftPresetIndex += 1;
      setLiftTarget(liftPresets[liftPresetIndex]);
    }
  } else if (liftDownBtn.changedToReleased()) {
    if (liftPresetIndex != 0) {
      liftPresetIndex -= 1;
      setLiftTarget(liftPresets[liftPresetIndex]);
    }
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
