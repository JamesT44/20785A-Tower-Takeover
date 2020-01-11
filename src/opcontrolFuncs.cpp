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
    // std::cout << "test\n";
    pros::delay(10);
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
