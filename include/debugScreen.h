#pragma once

#include "main.h"

static const char * odomBtnmMap[] = {"\202Reset", "\n",
                                  "\201X+", "\201Y+", ""};

static const okapi::QLength robotWidth = 12.5_in;
static const okapi::QLength robotLength = 18_in;

class DebugScreen {
  public:
    DebugScreen(lv_obj_t* parent, const std::shared_ptr<okapi::OdomChassisController> &robotOdom);
    ~DebugScreen();

    void updateOdom();
    std::shared_ptr<okapi::OdomChassisController> getRobotOdomController();

  private:
    std::shared_ptr<okapi::OdomChassisController> robotOdomController{nullptr};
    lv_obj_t* tabview;
    lv_obj_t* odomStatusLabel;
};

lv_res_t odomBtnCallback(lv_obj_t* btnm, const char* text);
