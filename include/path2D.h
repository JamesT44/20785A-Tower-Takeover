#pragma once

#include "point2D.h"

class Path2D {
  public:
  Path2D() = default;
  Path2D(const Path2D &path) = default;
  Path2D(Path2D &&path) = default;

  Path2D(const std::vector<Point2D> &path);
  Path2D(std::vector<Point2D> &&path);

  ~Path2D() = default;

  Path2D &operator=(const Path2D &path) = default;
  Path2D &operator=(Path2D &&path) = default;

  std::vector<std::shared_ptr<Point2D>> &getPoints();

  Path2D copy() const;

  void interpolate(const okapi::QLength &resolution);
  void smoothen(double weight, int iterations);

  Path2D &withInterpolation(const okapi::QLength &resolution);
  Path2D &withSmoothening(double weight, int iterations);

  protected:
  std::vector<std::shared_ptr<Point2D>> points;
};
