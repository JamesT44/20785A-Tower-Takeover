#pragma once

#include "customOdometry.h"
#include "main.h"

static const char *odomBtnmMap[] = {"\203Reset", "\n", "\201X+", "\201Y+", "\201Turn", ""};

static const double robotWidth = (12.5_in).convert(okapi::foot);
static const double robotLength = (18_in).convert(okapi::foot);
static const double centreDist = (3.5_in).convert(okapi::foot);

static const int fieldImgSize = 216;

class DebugScreen {
  public:
  DebugScreen(lv_obj_t *parent, const std::shared_ptr<okapi::OdomChassisController> &robotOdom);
  ~DebugScreen();

  void updateOdom();
  std::shared_ptr<okapi::OdomChassisController> getRobotController();

  protected:
  std::shared_ptr<okapi::OdomChassisController> robotController{nullptr};
  lv_obj_t *tabview;
  lv_obj_t *odomStatusLabel;
  lv_obj_t *robotLine;
  std::vector<lv_point_t> robotPoints = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
};

lv_res_t odomBtnCallback(lv_obj_t *btnm, const char *text);
