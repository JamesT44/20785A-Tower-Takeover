#pragma once

#include "customOdometry.h"
#include "main.h"

static const char *autonRollerOptions =
  "none\n1pt any goal\n5pt small goal\n8pt small goal\nskills";

static const char *odomBtnmMap[] = {"\203Reset", "\n", "\201X+", "\201Y+", "\201Turn", ""};

static const double robotWidth = (12.5_in).convert(okapi::foot);
static const double robotLength = (18_in).convert(okapi::foot);
static const double centreDist = (3.5_in).convert(okapi::foot);

static const double odomImgScale = 216.0 / 12.0;

class ScreenDisplay {
  public:
  ScreenDisplay(lv_obj_t *parent, const std::shared_ptr<okapi::OdomChassisController> &robotOdom);
  ~ScreenDisplay();

  void updateOdom();
  std::shared_ptr<okapi::OdomChassisController> getRobotController();

  protected:
  lv_style_t style_sw_blue;
  lv_style_t style_sw_red;

  std::shared_ptr<okapi::OdomChassisController> robotController{nullptr};
  lv_obj_t *tabview;
  lv_obj_t *odomStatusLabel;
  lv_obj_t *robotLine;
  std::vector<lv_point_t> robotPoints = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
};

lv_res_t odomBtnmCallback(lv_obj_t *btnm, const char *text);
