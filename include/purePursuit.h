#pragma once

#include "main.h"
#include "trajectory2D.h"

class PurePursuit {
  public:
  typedef std::pair<okapi::QAngularSpeed, okapi::QAngularSpeed> wheel_vels_t;

  PurePursuit(const std::shared_ptr<okapi::OdomChassisController> &controller,
              const okapi::QLength &lookahead,
              const okapi::TimeUtil &timeUtil = okapi::TimeUtilFactory().create());

  void executeTrajectory(const Trajectory2D &trajectory,
                         const okapi::QAcceleration &maxAccel,
                         bool backwards = false);

  protected:
  std::shared_ptr<okapi::OdomChassisController> controller{nullptr};

  const okapi::QLength lookahead;
  okapi::TimeUtil timeUtil;

  size_t prevNearest;
  double prevLookaheadIndex;

  size_t getNearestPoint(const Trajectory2D::points_t &points, const Point2D &currPosition);

  Point2D getLookaheadPoint(const Trajectory2D::points_t &points, const Point2D &currPosition);

  static double getFractionalIntersection(const Point2D &start,
                                          const Point2D &end,
                                          const Point2D &currPosition,
                                          const okapi::QLength &lookahead);

  okapi::QCurvature getCurvature(const okapi::OdomState &currPosition,
                                 const Point2D &lookaheadPoint);

  wheel_vels_t getWheelVels(const okapi::QSpeed &vel, const okapi::QCurvature &curvature);
};
