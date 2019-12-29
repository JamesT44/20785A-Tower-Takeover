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

  // lv_obj_t* odomLeftBar = lv_cont_create(odomTab, NULL);
  // lv_obj_set_pos(odomLeftBar, 5, -25);
  // lv_cont_set_layout(odomLeftBar, LV_LAYOUT_COL_L);
  // lv_cont_set_fit(odomLeftBar, true, true);

  odomStatusLabel = lv_label_create(odomTab, NULL);

  lv_obj_t* odomBtnm = lv_btnm_create(odomTab, NULL);
  lv_btnm_set_map(odomBtnm, odomBtnmMap);
  lv_obj_set_size(odomBtnm, 180, 80);
  lv_btnm_set_action(odomBtnm, odomBtnCallback);

  lv_obj_t* fieldImg = lv_img_create(odomTab, NULL);
  lv_img_set_src(fieldImg, &field_img);

  robotLine = lv_line_create(fieldImg, NULL);
  static lv_style_t robotLineStyle;
  lv_style_copy(&robotLineStyle, &lv_style_plain);
  robotLineStyle.line.color = LV_COLOR_BLACK;
  robotLineStyle.line.width = 3;
  lv_line_set_style(robotLine, &robotLineStyle);
  lv_obj_set_pos(robotLine, 0, 0);
  updateOdom();
  lv_obj_align(odomStatusLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  lv_obj_align(odomBtnm, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, -20);
  lv_obj_align(fieldImg, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
}

DebugScreen::~DebugScreen() {
  lv_obj_del(tabview);
}

void DebugScreen::updateOdom() {
  std::valarray<std::int32_t> encValues = robotOdomController->getModel()->getSensorVals();
  okapi::OdomState state = robotOdomController->getState();

  double x = state.x.convert(okapi::foot);
  double y = state.y.convert(okapi::foot);
  double theta = state.theta.convert(okapi::radian);

  std::string statusString = "L: " + std::to_string(encValues[0]) +
    "\nR: " + std::to_string(encValues[1]) +
    "\nM: " + std::to_string(encValues[2]) +
    "\nX: " + std::to_string(x) +
    "\nY: " + std::to_string(y) +
    "\nTheta: " + std::to_string(okapi::radianToDegree * theta);
  lv_label_set_text(odomStatusLabel, statusString.c_str());

  robotPoints[0] = {(short)(x / 12.0 * fieldImgSize),
                    (short)((1.0 - y / 12.0) * fieldImgSize)};

  x += centreDist * sin(theta);
  y += centreDist * cos(theta);
  robotPoints[1] = {(short)(x / 12.0 * fieldImgSize),
                    (short)((1.0 - y / 12.0) * fieldImgSize)};

  x -= robotWidth * cos(theta) / 2;
  y += robotWidth * sin(theta) / 2;
  robotPoints[2] = {(short)(x / 12.0 * fieldImgSize),
                    (short)((1.0 - y / 12.0) * fieldImgSize)};

  x -= robotLength * sin(theta);
  y -= robotLength * cos(theta);
  robotPoints[3] = {(short)(x / 12.0 * fieldImgSize),
                    (short)((1.0 - y / 12.0) * fieldImgSize)};

  x += robotWidth * cos(theta);
  y -= robotWidth * sin(theta);
  robotPoints[4] = {(short)(x / 12.0 * fieldImgSize),
                    (short)((1.0 - y / 12.0) * fieldImgSize)};

  x += robotLength * sin(theta);
  y += robotLength * cos(theta);
  robotPoints[5] = {(short)(x / 12.0 * fieldImgSize),
                    (short)((1.0 - y / 12.0) * fieldImgSize)};

  robotPoints[6] = robotPoints[1];
  lv_line_set_points(robotLine, robotPoints.data(), robotPoints.size());
}

std::shared_ptr<okapi::OdomChassisController> DebugScreen::getRobotOdomController() {
    return robotOdomController;
}

lv_res_t odomBtnCallback(lv_obj_t* btnm, const char* text) {
  okapi::OdomState state = chassisControl->getState();

  if (strcmp(text, "Reset") == 0) {
    chassisControl->getModel()->resetSensors();
    pros::delay(20);

    chassisControl->setState({0_ft, 0_ft, 0_deg});
  } else if (strcmp(text, "X+") == 0) {
    state.x += 2_ft;
    chassisControl->setState(state);
  } else if (strcmp(text, "Y+") == 0) {
    state.y += 2_ft;
    chassisControl->setState(state);
  } else if (strcmp(text, "Turn") == 0) {
    state.theta += 90_deg;
    chassisControl->setState(state);
  }
  return LV_RES_OK;
}
