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

  std::vector<std::shared_ptr<Point2D>> &getVector();

  Path2D copy() const;

  Path2D &interpolate(const okapi::QLength &resolution);
  Path2D &smoothen(double weight, const okapi::QLength &tolerance);

  protected:
  std::vector<std::shared_ptr<Point2D>> points;
};
