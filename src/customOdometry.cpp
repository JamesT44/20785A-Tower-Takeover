#include "customOdometry.h"
#include "main.h"
#include <cmath>

CustomOdometry::CustomOdometry(
  const okapi::TimeUtil &itimeUtil,
  const std::shared_ptr<okapi::ReadOnlyChassisModel> &imodel,
  const okapi::ChassisScales &ichassisScales,
  const std::shared_ptr<okapi::Logger> &ilogger)
  : logger(ilogger),
    rate(itimeUtil.getRate()),
    timer(itimeUtil.getTimer()),
    model(imodel),
    chassisScales(ichassisScales) {
}

void CustomOdometry::setScales(
  const okapi::ChassisScales &ichassisScales) {
  chassisScales = ichassisScales;
}

void CustomOdometry::step() {
  const auto deltaT = timer->getDt();

  if (deltaT.getValue() != 0) {
    newTicks = model->getSensorVals();
    tickDiff = newTicks - lastTicks;
    lastTicks = newTicks;

    const auto newState = odomMathStep(tickDiff, deltaT);

    state.x += newState.x;
    state.y += newState.y;
    state.theta += newState.theta;
  }
}

okapi::OdomState
CustomOdometry::odomMathStep(const std::valarray<std::int32_t> &itickDiff,
                             const okapi::QTime &) {
  if (itickDiff.size() < 3) {
    LOG_ERROR_S(
      "CustomOdometry: itickDiff did not have at least two elements.");
    return okapi::OdomState{};
  }

  for (auto &&elem : itickDiff) {
    if (std::abs(elem) > maximumTickDiff) {
      LOG_ERROR("CustomOdometry: A tick diff (" + std::to_string(elem) +
                ") was greater than the maximum allowable diff (" +
                std::to_string(maximumTickDiff) +
                "). Skipping this odometry step.");
      return okapi::OdomState{};
    }
  }

  // Calculate distance travelled by wheels
  const okapi::QLength deltaL =
    itickDiff[0] / chassisScales.tpr * chassisScales.wheelDiameter * 1_pi;
  const okapi::QLength deltaR =
    itickDiff[1] / chassisScales.tpr * chassisScales.wheelDiameter * 1_pi;
  const okapi::QLength deltaM = itickDiff[2] / chassisScales.tpr *
                                chassisScales.middleWheelDiameter * 1_pi;

  // Calculate angle travelled
  double deltaTheta =
    ((deltaL - deltaR) / chassisScales.wheelTrack).convert(okapi::number);

  // Calculate displacement of middle wheel
  const okapi::QLength deltaMiddle =
    deltaM - deltaTheta * chassisScales.middleWheelDistance;

  // Calculate robot-centric local offset
  okapi::QLength localOffX, localOffY;
  if (deltaL == deltaR) {
    localOffX = deltaM;
    localOffY = deltaR;
  } else {
    localOffX =
      2 * sin(deltaTheta / 2) *
      (deltaMiddle / deltaTheta + chassisScales.middleWheelDistance * 2);
    localOffY = 2 * sin(deltaTheta / 2) *
                (deltaR / deltaTheta + chassisScales.wheelTrack / 2);
  }

  // Find average heading during motion
  double avgA = state.theta.convert(okapi::radian) + (deltaTheta / 2);

  // Convert local offset to global offset (by converting to polar
  // coordinates)
  okapi::QLength polarR =
    ((localOffX * localOffX) + (localOffY * localOffY)).sqrt();
  double polarA = atan2(localOffY.convert(okapi::meter),
                        localOffX.convert(okapi::meter)) -
                  avgA;
  okapi::QLength dX = sin(polarA) * polarR;
  okapi::QLength dY = cos(polarA) * polarR;

  if (isnan(dX.convert(okapi::meter))) {
    dX = 0_m;
  }

  if (isnan(dY.convert(okapi::meter))) {
    dY = 0_m;
  }

  if (isnan(deltaTheta)) {
    deltaTheta = 0;
  }

  return okapi::OdomState{dX, dY, deltaTheta * okapi::radian};
}

okapi::OdomState
CustomOdometry::getState(const okapi::StateMode &imode) const {
  if (imode == okapi::StateMode::FRAME_TRANSFORMATION) {
    return state;
  } else {
    return okapi::OdomState{state.y, state.x, state.theta};
  }
}

void CustomOdometry::setState(const okapi::OdomState &istate,
                              const okapi::StateMode &imode) {
  LOG_DEBUG("State set to: " + istate.str());
  if (imode == okapi::StateMode::FRAME_TRANSFORMATION) {
    state = istate;
  } else {
    state = okapi::OdomState{istate.y, istate.x, istate.theta};
  }
}

std::shared_ptr<okapi::ReadOnlyChassisModel> CustomOdometry::getModel() {
  return model;
}

okapi::ChassisScales CustomOdometry::getScales() {
  return chassisScales;
}
