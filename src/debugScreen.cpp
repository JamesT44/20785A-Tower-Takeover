#include "main.h"
#include "debugScreen.h"
#include "deviceConfig.h"

LV_IMG_DECLARE(field_img);

DebugScreen::DebugScreen(
  lv_obj_t* parent,
  const std::shared_ptr<okapi::OdomChassisController> &robotOdom)
  : tabview(lv_tabview_create(parent, NULL)),
    robotOdomController(robotOdom) {

  lv_tabview_set_btns_hidden(tabview, true);
  lv_obj_t* odomTab = lv_tabview_add_tab(tabview, "Odometry");
  lv_obj_t* motorTab = lv_tabview_add_tab(tabview, "Motors");

  lv_obj_t* odomLeftBar = lv_cont_create(odomTab, NULL);
  lv_cont_set_layout(odomLeftBar, LV_LAYOUT_COL_M);
  lv_cont_set_fit(odomLeftBar, true, true);

  odomStatusLabel = lv_label_create(odomLeftBar, NULL);
  updateOdom();

  lv_obj_t* odomBtnm = lv_btnm_create(odomLeftBar, NULL);
  lv_btnm_set_map(odomBtnm, odomBtnmMap);
  lv_obj_set_size(odomBtnm, 120, 60);
  lv_obj_align(odomLeftBar, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
  lv_btnm_set_action(odomBtnm, odomBtnCallback);

  lv_obj_t* fieldImg = lv_img_create(odomTab, NULL);
  lv_img_set_src(fieldImg, &field_img);
  lv_obj_align(fieldImg, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
}

DebugScreen::~DebugScreen() {
  lv_obj_del(tabview);
}

void DebugScreen::updateOdom() {
  std::valarray<std::int32_t> encValues = robotOdomController->getModel()->getSensorVals();
  okapi::OdomState state = robotOdomController->getState();
  std::string statusString = "L: " + std::to_string(encValues[0]) +
    "\nR: " + std::to_string(encValues[1]) +
    "\nM: " + std::to_string(encValues[2]) +
    "\nX: " + std::to_string(state.x.convert(okapi::foot)) +
    "\nY: " + std::to_string(state.y.convert(okapi::foot)) +
    "\nTheta: " + std::to_string(state.theta.convert(okapi::degree));
  lv_label_set_text(odomStatusLabel, statusString.c_str());
}

std::shared_ptr<okapi::OdomChassisController> DebugScreen::getRobotOdomController() {
    return robotOdomController;
}

lv_res_t odomBtnCallback(lv_obj_t* btnm, const char* text) {
  DebugScreen* that = static_cast<DebugScreen*>(lv_obj_get_free_ptr(btnm));
  if (strcmp(text, "Reset") == 0) {
    chassisControl->getModel()->resetSensors();
    pros::delay(20);
    chassisControl->setState({0_ft, 0_ft, 0_deg});
  } else if (strcmp(text, "X+") == 0) {
    okapi::OdomState state = chassisControl->getState();
    state.x += 2_ft;
    chassisControl->setState(state);
  } else if (strcmp(text, "Y+") == 0) {
    okapi::OdomState state = chassisControl->getState();
    state.y += 2_ft;
    chassisControl->setState(state);
  }
  return LV_RES_OK;
}
