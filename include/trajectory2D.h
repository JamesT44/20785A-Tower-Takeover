#pragma once

#include "okapi/api/units/RQuantity.hpp"
#include "path2D.h"

namespace okapi {
QUANTITY_TYPE(0, -1, 0, 0, QCurvature)

constexpr QCurvature mcrvt = 1 / meter;

namespace literals {
constexpr QCurvature operator"" _mcrvt(long double x) {
  return QCurvature(x);
}

} // namespace literals
} // namespace okapi

class Trajectory2D : public Path2D {
  public:
  enum class SDCardRes { success = 0, noSDCard = 1, cannotOpenFile = 2, invalidDataFormat = 3 };

  Trajectory2D() = default;
  Trajectory2D(const Trajectory2D &path) = default;
  Trajectory2D(Trajectory2D &&path) = default;

  Trajectory2D(const std::vector<Point2D> &path);
  Trajectory2D(std::vector<Point2D> &&path);

  ~Trajectory2D() = default;

  Trajectory2D &operator=(const Trajectory2D &path) = default;
  Trajectory2D &operator=(Trajectory2D &&path) = default;

  std::vector<okapi::QCurvature> &getCurvatures();
  std::vector<okapi::QSpeed> &getVelocities();

  Trajectory2D copy() const;

  Trajectory2D &generateTrajectory(const okapi::QSpeed &maxVel,
                                   const okapi::QAcceleration &maxDecel,
                                   const okapi::QFrequency &kTurn);

  Trajectory2D &withInterpolation(const okapi::QLength &resolution);
  Trajectory2D &withSmoothening(double weight, int iterations);

  SDCardRes saveToSD(const std::string &identifier);
  SDCardRes loadFromSD(const std::string &identifier);

  protected:
  std::vector<okapi::QCurvature> curvatures;
  std::vector<okapi::QSpeed> velocities;

  void setCurvatures();
  void setVelocities(const okapi::QSpeed &maxVel,
                     const okapi::QAcceleration &maxDecel,
                     const okapi::QFrequency &kTurn);

  bool SDCardInserted();
};
