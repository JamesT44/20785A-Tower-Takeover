#include "odompidcontroller.hpp"

OdomPIDController::OdomPIDController(
  const std::shared_ptr<okapi::ChassisModel> &model,
  const std::shared_ptr<CustomOdometry> &odometry,
  std::unique_ptr<okapi::IterativePosPIDController> distanceController,
  std::unique_ptr<okapi::IterativePosPIDController> turnController,
  std::unique_ptr<okapi::IterativePosPIDController> angleController,
  const okapi::TimeUtil &timeUtil)
  : model(std::move(model)),
    odometry(std::move(odometry)),
    distanceController(std::move(distanceController)),
    angleController(std::move(angleController)),
    turnController(std::move(turnController)),
    timeUtil(timeUtil){};

void OdomPIDController::turnToAngle(const okapi::QAngle &angle,
                                    double leftBias) {
  resetPid();
  auto rate = timeUtil.getRate();
  do {
    angleErr = angle - odometry->getState().theta;
    double vel = turnController->step(-angleErr.convert(okapi::degree));
    model->tank(leftBias * vel, (2 - leftBias) * vel);
    rate->delayUntil(10_ms);
  } while (!isTurnSettled());
  model->tank(0, 0);
}

void OdomPIDController::turnAngle(const okapi::QAngle &angle,
                                  double leftBias) {
  turnToAngle(odometry->getState().theta + angle);
}

void OdomPIDController::moveDistance(const okapi::QLength &distance) {
  resetPid();
  auto rate = timeUtil.getRate();
  auto lastTicks = model->getSensorVals();
  auto bearing = odometry->getState().theta;
  do {
    auto newTicks = model->getSensorVals();
    okapi::QLength leftDistance =
      ((newTicks[0] - lastTicks[0]) / odometry->getScales().straight) *
      okapi::meter;
    okapi::QLength rightDistance =
      ((newTicks[1] - lastTicks[1]) / odometry->getScales().straight) *
      okapi::meter;

    distanceErr = distance - ((leftDistance + rightDistance) / 2);
    angleErr = bearing - odometry->getState().theta;

    double distanceVel =
      distanceController->step(-distanceErr.convert(okapi::millimeter));
    double angleVel =
      angleController->step(-angleErr.convert(okapi::degree));

    model->driveVector(distanceVel, angleVel);
    rate->delayUntil(10_ms);
  } while (!(isDistanceSettled() && isAngleSettled()));

  model->driveVector(0, 0);
}

void OdomPIDController::driveToPoint(const okapi::Point &targetPoint) {
  auto [distance, angle] = okapi::OdomMath::computeDistanceAndAngleToPoint(
    targetPoint, odometry->getState());
  turnAngle(angle);
  moveDistance(distance);
}

bool OdomPIDController::isDistanceSettled() const {
  return distanceController->isSettled();
}

bool OdomPIDController::isAngleSettled() const {
  return angleController->isSettled();
}

bool OdomPIDController::isTurnSettled() const {
  return turnController->isSettled();
}

void OdomPIDController::resetPid() {
  distanceController->reset();
  turnController->reset();
  angleController->reset();
  distanceErr = 0_in;
  angleErr = 0_deg;
}
