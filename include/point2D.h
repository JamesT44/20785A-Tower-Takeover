#pragma once

#include "main.h"

/**
 * A 2D Point with some basic operations
 */
class Point2D {
  public:
  okapi::QLength x{0_ft};
  okapi::QLength y{0_ft};

  Point2D() = default;
  Point2D(const Point2D &point) = default;
  ~Point2D() = default;

  Point2D(const okapi::QLength &x, const okapi::QLength &y);
  Point2D(const okapi::OdomState &state);

  okapi::QLength &operator[](size_t index);
  const okapi::QLength &operator[](size_t index) const;

  Point2D operator+(const Point2D &rhs) const;
  Point2D operator-(const Point2D &rhs) const;
  Point2D operator*(const double rhs) const;
  Point2D operator/(const double rhs) const;

  bool operator==(const Point2D &rhs) const;
  bool operator!=(const Point2D &rhs) const;

  static okapi::QLength dist(const Point2D &lhs, const Point2D &rhs);
  okapi::QLength dist(const Point2D &point);

  static okapi::QLength magnitude(const Point2D &point);
  okapi::QLength magnitude();

  static okapi::QArea dot(const Point2D &lhs, const Point2D &rhs);
  okapi::QArea dot(const Point2D &point);
};
