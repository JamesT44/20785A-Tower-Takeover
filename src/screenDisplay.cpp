#include "screenDisplay.h"
#include "deviceConfig.h"
#include "main.h"

LV_IMG_DECLARE(auton_field_img);
LV_IMG_DECLARE(odom_field_img);

ScreenDisplay::ScreenDisplay(lv_obj_t *parent,
                             const std::shared_ptr<okapi::OdomChassisController> &robotOdom)
  : tabview(lv_tabview_create(parent, NULL)), robotController(robotOdom) {
  lv_tabview_set_btns_hidden(tabview, true);

  lv_obj_t *autonTab = lv_tabview_add_tab(tabview, "Autonomous");
  lv_obj_t *autonTabLeft = lv_cont_create(autonTab, NULL);
  lv_cont_set_layout(autonTabLeft, LV_LAYOUT_PRETTY);

  lv_obj_t *allianceLabel = lv_label_create(autonTabLeft, NULL);
  lv_label_set_text(allianceLabel, "Alliance: ");

  lv_obj_t *allianceSw = lv_sw_create(autonTabLeft, NULL);
  static lv_style_t allianceSwStyle;
  lv_style_copy(&style_sw_blue, &lv_style_pretty);
  style_sw_blue.body.main_color = LV_COLOR_BLUE;
  style_sw_blue.body.grad_color = LV_COLOR_MAKE(0x00, 0x2A, 0xFF);
  lv_style_copy(&style_sw_red, &lv_style_pretty);
  style_sw_red.body.main_color = LV_COLOR_RED;
  style_sw_red.body.grad_color = LV_COLOR_MAKE(0xFF, 0x2A, 0x00);
  lv_sw_set_style(allianceSw, LV_SW_STYLE_KNOB_OFF, &style_sw_blue);
  lv_sw_set_style(allianceSw, LV_SW_STYLE_KNOB_ON, &style_sw_red);
  lv_sw_set_style(allianceSw, LV_SW_STYLE_INDIC, &lv_style_transp);

  lv_obj_t *autonRoller = lv_roller_create(autonTab, NULL);
  lv_roller_set_options(autonRoller, autonRollerOptions);
  lv_roller_set_selected(autonRoller, 2, false);

  lv_obj_t *autonImg = lv_img_create(autonTab, NULL);
  lv_img_set_src(autonImg, &auton_field_img);

  lv_obj_align(autonTabLeft, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
  lv_obj_align(autonImg, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

  lv_obj_t *odomTab = lv_tabview_add_tab(tabview, "Odometry");

  odomStatusLabel = lv_label_create(odomTab, NULL);

  lv_obj_t *odomBtnm = lv_btnm_create(odomTab, NULL);
  lv_btnm_set_map(odomBtnm, odomBtnmMap);
  lv_obj_set_size(odomBtnm, 180, 80);
  lv_btnm_set_action(odomBtnm, odomBtnmCallback);

  lv_obj_t *odomImg = lv_img_create(odomTab, NULL);
  lv_img_set_src(odomImg, &odom_field_img);

  robotLine = lv_line_create(odomImg, NULL);
  static lv_style_t robotLineStyle;
  lv_style_copy(&robotLineStyle, &lv_style_plain);
  robotLineStyle.line.color = LV_COLOR_BLACK;
  robotLineStyle.line.width = 3;
  lv_line_set_style(robotLine, &robotLineStyle);
  lv_obj_set_pos(robotLine, 0, 0);

  updateOdom();

  lv_obj_align(odomStatusLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  lv_obj_align(odomBtnm, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, -20);
  lv_obj_align(odomImg, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
}

ScreenDisplay::~ScreenDisplay() {
  lv_obj_del(tabview);
}

void ScreenDisplay::updateOdom() {

  std::valarray<std::int32_t> encValues = robotController->getModel()->getSensorVals();
  okapi::OdomState state = robotController->getState();

  double x = state.x.convert(okapi::foot);
  double y = state.y.convert(okapi::foot);
  double theta = state.theta.convert(okapi::radian);

  std::string statusString =
    "L: " + std::to_string(encValues[0]) + "\nR: " + std::to_string(encValues[1]) +
    "\nM: " + std::to_string(encValues[2]) + "\nX: " + std::to_string(x) +
    "\nY: " + std::to_string(y) + "\nTheta: " + std::to_string(okapi::radianToDegree * theta);
  lv_label_set_text(odomStatusLabel, statusString.c_str());
  lv_obj_invalidate(odomStatusLabel);

  double sinTheta = sin(theta);
  double cosTheta = cos(theta);
  robotPoints[0] = {(short)(x * odomImgScale), (short)((1.0 - y) * odomImgScale)};

  x += centreDist * sinTheta;
  y += centreDist * cosTheta;
  robotPoints[1] = {(short)(x * odomImgScale), (short)((1.0 - y) * odomImgScale)};

  x -= robotWidth * cosTheta / 2;
  y += robotWidth * sinTheta / 2;
  robotPoints[2] = {(short)(x * odomImgScale), (short)((1.0 - y) * odomImgScale)};

  x -= robotLength * sinTheta;
  y -= robotLength * cosTheta;
  robotPoints[3] = {(short)(x * odomImgScale), (short)((1.0 - y) * odomImgScale)};

  x += robotWidth * cosTheta;
  y -= robotWidth * sinTheta;
  robotPoints[4] = {(short)(x * odomImgScale), (short)((1.0 - y) * odomImgScale)};

  x += robotLength * sinTheta;
  y += robotLength * cosTheta;
  robotPoints[5] = {(short)(x * odomImgScale), (short)((1.0 - y) * odomImgScale)};

  robotPoints[6] = robotPoints[1];
  lv_line_set_points(robotLine, robotPoints.data(), robotPoints.size());
  // lv_obj_invalidate(robotLine);
}

std::shared_ptr<okapi::OdomChassisController> ScreenDisplay::getRobotController() {
  return robotController;
}

lv_res_t odomBtnmCallback(lv_obj_t *btnm, const char *text) {
  okapi::OdomState state = chassisControl->getState();

  if (strcmp(text, "Reset") == 0) {
    robotModel->resetSensors();
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
