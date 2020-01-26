#include "purePursuit.h"

PurePursuit::PurePursuit(const std::shared_ptr<okapi::OdomChassisController> &controller,
                         const okapi::QLength &lookahead,
                         const okapi::TimeUtil &timeUtil)
  : controller(controller), lookahead(lookahead), timeUtil(timeUtil) {
  controller->setDefaultStateMode(okapi::StateMode::CARTESIAN);
}

void PurePursuit::executeTrajectory(const Trajectory2D &trajectory,
                                    const okapi::QAcceleration &maxAccel,
                                    bool backwards) {
  auto rate = timeUtil.getRate();
  auto timer = timeUtil.getTimer();

  auto points = trajectory.getPointsVector();
  auto end = points.back();

  prevNearest = 0;
  prevLookaheadIndex = 0.0;

  okapi::QSpeed lastVelocity = 0_mps;

  bool finished = false;
  while (!finished) {
    okapi::OdomState currPosition = controller->getState();

    auto currNearest = getNearestPoint(points, currPosition);
    Point2D lookaheadPoint = getLookaheadPoint(points, currPosition);

    bool endWithinLookahead = end->dist(*trajectory.getPointsVector()[currNearest]) < lookahead &&
                              end->dist(currPosition) < lookahead;

    okapi::QCurvature curvature =
      endWithinLookahead ? 0.0_mcrvt : getCurvature(currPosition, lookaheadPoint);

    okapi::QAngle angleToEnd =
      std::fmod(
        okapi::OdomMath::computeAngleToPoint({end->x, end->y}, currPosition).convert(okapi::degree),
        360) *
      okapi::degree;

    bool pastEnd = (angleToEnd >= 90_deg && angleToEnd <= 270_deg) ^ backwards;

    if (endWithinLookahead) {
      backwards = angleToEnd >= 90_deg && angleToEnd <= 270_deg;
    }

    finished = pastEnd && endWithinLookahead;

    auto dt = timer->getDt();
    okapi::QSpeed accelLimited = lastVelocity + dt * maxAccel;

    okapi::QSpeed targetVel = std::min(trajectory.getVelocitiesVector()[currNearest], accelLimited);
    lastVelocity = targetVel;

    auto wheelVels = getWheelVels(targetVel, curvature);

    if (backwards) {
      controller->getModel()->tank(-wheelVels.first.convert(okapi::rpm) / 200,
                                   -wheelVels.second.convert(okapi::rpm) / 200);
    } else {
      controller->getModel()->tank(wheelVels.first.convert(okapi::rpm) / 200,
                                   wheelVels.second.convert(okapi::rpm) / 200);
    }

    rate->delayUntil(10_ms);
  }

  controller->getModel()->driveVector(0, 0);
}

size_t PurePursuit::getNearestPoint(const Trajectory2D::points_t &points,
                                    const Point2D &currPosition) {
  okapi::QLength nearestDist{std::numeric_limits<double>::max()};
  auto nearest = prevNearest;
  size_t end =
    points.back()->dist(currPosition) > lookahead ? (int)prevLookaheadIndex + 2 : points.size();
  for (size_t i = nearest; i < end; i++) {
    std::cout << points.size() << " b\n";
    okapi::QLength distance = points[i]->dist(currPosition);
    std::cout << i << " c\n";
    if (distance < nearestDist) {
      std::cout << i << " d\n";
      nearestDist = distance;
      std::cout << i << " e\n";
      nearest = i;
      std::cout << i << " f\n";
    }
  }

  prevNearest = nearest;
  return nearest;
}

Point2D PurePursuit::getLookaheadPoint(const Trajectory2D::points_t &points,
                                       const Point2D &currPosition) {
  size_t lastIntersect = 0;

  for (size_t i = std::max((size_t)prevLookaheadIndex, prevNearest); i < points.size() - 1; i++) {
    auto &start = *points[i];
    auto &end = *points[i + 1];

    double frac = getFractionalIntersection(start, end, currPosition, lookahead);
    if (frac != -1.0) {
      if (i + frac > prevLookaheadIndex) {
        prevLookaheadIndex = i;
        if (lastIntersect > 0) {
          break;
        }
        lastIntersect = i;
      }
    }

    if (lastIntersect > 0 && points[i]->dist(*points[lastIntersect]) >= lookahead * 2) {
      break;
    }
  }

  int intPart = prevLookaheadIndex;
  double fracPart = prevLookaheadIndex - intPart;

  auto &start = *points[intPart];
  auto &end = *points[intPart + 1];
  return start + ((end - start) * fracPart);
}

double PurePursuit::getFractionalIntersection(const Point2D &start,
                                              const Point2D &end,
                                              const Point2D &currPosition,
                                              const okapi::QLength &lookahead) {
  Point2D d = end - start;
  Point2D f = start - currPosition;

  okapi::QArea a = d.dot(d);
  okapi::QArea b = d.dot(f) * 2.0;
  okapi::QArea c = f.dot(f) - lookahead * lookahead;
  auto discriminant = b * b - (4.0 * (a * c));

  if (discriminant.getValue() >= 0) {
    okapi::QArea sqrt_discriminant = discriminant.sqrt();
    double t1 = ((-b - sqrt_discriminant) / (2.0 * a)).convert(okapi::number);
    double t2 = ((-b + sqrt_discriminant) / (2.0 * a)).convert(okapi::number);

    if (t2 >= 0.0 && t2 <= 1.0) {
      return t2;
    } else if (t1 >= 0.0 && t1 <= 1.0) {
      return t1;
    }
  }

  return -1.0;
}

okapi::QCurvature PurePursuit::getCurvature(const okapi::OdomState &currPosition,
                                            const Point2D &lookaheadPoint) {
  Point2D diff = lookaheadPoint - currPosition;
  double angle = ((currPosition.theta * -1) + 90_deg).convert(okapi::radian);
  okapi::QLength l = Point2D::dist(lookaheadPoint, currPosition);

  double a = -std::tan(angle);
  okapi::QLength c = -a * currPosition.x - currPosition.y;
  okapi::QLength x = (a * lookaheadPoint.x + lookaheadPoint.y + c).abs() / std::sqrt(a * a + 1);

  int side = (std::sin(angle) * diff.x - std::cos(angle) * diff.y) >= 0_m ? 1 : -1;

  okapi::QCurvature curv = (2.0 * x) / (l * l);
  return curv * side;
}

PurePursuit::wheel_vels_t PurePursuit::getWheelVels(const okapi::QSpeed &vel,
                                                    const okapi::QCurvature &curvature) {
  okapi::QLength trackWidth = controller->getChassisScales().wheelTrack;
  okapi::QSpeed leftVel = vel * (2.0 * okapi::number + trackWidth * curvature) / 2.0;
  okapi::QSpeed rightVel = vel * (2.0 * okapi::number - trackWidth * curvature) / 2.0;

  okapi::QLength wheelDiam = controller->getChassisScales().wheelDiameter;
  okapi::QAngularSpeed leftWheel = (leftVel / (1_pi * wheelDiam)) * 360_deg;
  okapi::QAngularSpeed rightWheel = (rightVel / (1_pi * wheelDiam)) * 360_deg;

  return {leftWheel, rightWheel};
}
