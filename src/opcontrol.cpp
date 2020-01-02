#include "debugScreen.h"
#include "deviceConfig.h"
#include "main.h"
#include "opcontrolFuncs.h"
#include "path2D.h"
#include "point2D.h"
#include <fstream>

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
  // int i = 0;
  // Path2D path = Path2D({{0_ft, 0_ft}, {0_ft, 2_ft}, {2_ft, 2_ft}, {2_ft, 4_ft}})
  //                 .interpolate(1_cm)
  //                 .smoothen(.001, 1e-10_m);
  // std::vector<std::shared_ptr<Point2D>> points = path.getVector();
  //
  // std::ofstream file;
  // file.open("/usd/path.txt");
  // for (auto i = points.begin(); i != points.end(); ++i)
  //   file << (*i)->x.convert(okapi::foot) << " " << (*i)->y.convert(okapi::foot) << '\n';
  // file.close();

  while (true) {
    // Abstracted into functions
    chassisOpcontrol();
    tilterOpcontrol();
    intakeOpcontrol();

    debugDisplay.updateOdom();

    // if (i++ == 50) {
    //   std::cout << chassisControl->getState().str() << std::endl;
    //   i = 0;
    // }
    pros::delay(10);
  }
}
