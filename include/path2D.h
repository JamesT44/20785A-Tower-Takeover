#pragma once

#include "point2D.h"

/**
 * A 2D Path consisting of Point2D's with interpolation and smoothening
 * methods
 */
class Path2D {
  public:
  typedef std::vector<std::shared_ptr<Point2D>> points_t;

  /**
   * Default ctors, dtor and assignment operators
   */
  Path2D() = default;
  Path2D(const Path2D &path) = default;
  Path2D(Path2D &&path) = default;
  ~Path2D() = default;
  Path2D &operator=(const Path2D &path) = default;
  Path2D &operator=(Path2D &&path) = default;

  /**
   * Convert a vector of points to a Path2D
   * @param path The vector of points
   */
  Path2D(const std::vector<Point2D> &path);
  Path2D(std::vector<Point2D> &&path);

  /**
   * @return The internal vector of Point2D's
   */
  const points_t &getPointsVector() const;
  points_t &pointsVector();

  /**
   * Return a copy of this Path2D
   * @return The copy
   */
  Path2D copy() const;

  /**
   * Interpolate a path consisting of waypoints. Uses linear interpolation
   * on each pair of adjacent points, so distance between adjacent
   * interpolated points is not constant
   * @param resolution Maximum distance between adjacent point
   */
  void interpolate(const okapi::QLength &resolution);

  /**
   * Smoothen the path using gradient descent. Adapted from FRC team 2168's
   * code.
   * @param weight     How closely to stick to original points (0.0 - 1.0)
   * @param iterations How many iterations to smooth for
   */
  void smoothen(double weight, int iterations);

  /**
   * Allow method chaining for ease of use
   */
  Path2D &withInterpolation(const okapi::QLength &resolution);
  Path2D &withSmoothening(double weight, int iterations);

  protected:
  /**
   * The vector of points making up the path
   */
  points_t points;
};
