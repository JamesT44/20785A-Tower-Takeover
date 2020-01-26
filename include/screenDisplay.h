#pragma once

#include "customOdometry.h"
#include "main.h"

LV_IMG_DECLARE(auton_blue_1pt);
LV_IMG_DECLARE(auton_red_1pt);
LV_IMG_DECLARE(auton_blue_5pt_small);
LV_IMG_DECLARE(auton_red_5pt_small);
LV_IMG_DECLARE(auton_blue_8pt_small);
LV_IMG_DECLARE(auton_red_8pt_small);
LV_IMG_DECLARE(auton_none);
LV_IMG_DECLARE(odom_field_img);

static const char *autonOptionStrs[] = {"none",
                                        "1pt any goal",
                                        "5pt small goal",
                                        "8pt small goal",
                                        "skills"};
static const lv_img_dsc_t *autonOptionImgs[][5] = {{&auton_none,
                                                    &auton_blue_1pt,
                                                    &auton_blue_5pt_small,
                                                    &auton_blue_8pt_small,
                                                    &auton_none},
                                                   {&auton_none,
                                                    &auton_red_1pt,
                                                    &auton_red_5pt_small,
                                                    &auton_red_8pt_small,
                                                    &auton_none}};

static const char *odomBtnmMap[] =
  {"\203Reset", "\n", "\201X+", "\201Y+", "\201Turn", ""};

static const double robotWidth = (12.5_in).convert(okapi::foot);
static const double robotLength = (18_in).convert(okapi::foot);
static const double centreDist = (3.5_in).convert(okapi::foot);

static const double odomImgScale = 216.0 / 12.0;

class ScreenDisplay {
  public:
  /**
   * Main GUI for the robot. Contains multiple tabs and there should only
   * be one instance of this class.
   * @param parent    the lvgl object to draw the screen within
   * @param robotOdom the odomChassisController instance to use for the
   * odomTab
   */
  ScreenDisplay(
    lv_obj_t *parent,
    const std::shared_ptr<okapi::OdomChassisController> &robotOdom);

  ~ScreenDisplay();

  /**
   * @return The autonomous selection. The bool is true if red. The char *
   * is the string of the currently selected routine.
   */
  std::pair<bool, char *> getAutonSelection();

  /**
   * Refresh the auton tab. Redraws the image.
   */
  void updateAuton();

  /**
   * Refresh the odometry tab. Redraws the label and robot line.
   */
  void updateOdom();

  /**
   * @return The internal OdomChassisController
   */
  std::shared_ptr<okapi::OdomChassisController> getRobotController();

  protected:
  lv_obj_t *allianceSw;
  lv_obj_t *autonRoller;
  lv_obj_t *autonImg;
  lv_style_t style_sw_blue;
  lv_style_t style_sw_red;

  std::shared_ptr<okapi::OdomChassisController> robotController{nullptr};
  lv_obj_t *tabview;
  lv_obj_t *odomStatusLabel;
  lv_obj_t *robotLine;
  std::vector<lv_point_t> robotPoints =
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
};

/**
 * Callback action for the button matrix on the odometry tabs
 * @param  btnm the button matrix that was pressed
 * @param  text the label on the button that was pressed
 * @return      result of callback
 */
lv_res_t odomBtnmCallback(lv_obj_t *btnm, const char *text);
