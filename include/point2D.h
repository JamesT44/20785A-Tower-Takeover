#pragma once
#include "main.h"

class Point2D {
  public:
  okapi::QLength x{0_ft};
  okapi::QLength y{0_ft};

  Point2D() = default;
  Point2D(const Point2D &point) = default;
  ~Point2D() = default;

  Point2D(const okapi::QLength &x, const okapi::QLength &y);

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
};
