#pragma once

#include "point2D.h"

class Path2D {
  public:
  typedef std::vector<std::shared_ptr<Point2D>> points_t;

  Path2D() = default;
  Path2D(const Path2D &path) = default;
  Path2D(Path2D &&path) = default;

  Path2D(const std::vector<Point2D> &path);
  Path2D(std::vector<Point2D> &&path);

  ~Path2D() = default;

  Path2D &operator=(const Path2D &path) = default;
  Path2D &operator=(Path2D &&path) = default;

  const points_t &getPointsVector() const;
  points_t &pointsVector();

  Path2D copy() const;

  void interpolate(const okapi::QLength &resolution);
  void smoothen(double weight, int iterations);

  Path2D &withInterpolation(const okapi::QLength &resolution);
  Path2D &withSmoothening(double weight, int iterations);

  protected:
  points_t points;
};
