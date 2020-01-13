#include "deviceConfig.h"
#include "main.h"
#include "opcontrolFuncs.h"
#include "pros/apix.h"
#include "pros/rtos.hpp"
#include "purePursuit.h"
#include "screenDisplay.h"
#include "trajectory2D.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  auto start = pros::millis();
  pros::delay(500);

  // auto path = Trajectory2D();
  // auto res = path.loadFromSD("test");
  // std::cout << "res: " << (int)res << std::endl;
  // std::cout << "elapsed: " << pros::millis() - start << std::endl;
  // path.saveToSD("test2");
  std::string s = "Hello world";
  pros::Task chassisTask(chassisOpcontrolTask, nullptr, "");
  int i = 0;
  while (true) {
    // Abstracted into functions
    liftOpcontrol();
    tilterOpcontrol();
    intakeOpcontrol();

    mainDisplay.updateOdom();

    if (i++ == 50) {
      std::cout << chassisControl->getState().str() << std::endl;
      i = 0;
    }
    pros::delay(10);
  }
}
