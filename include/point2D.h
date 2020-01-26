#pragma once

#include "main.h"

/**
 * A 2D Point with some basic operations
 */
class Point2D {
  public:
  okapi::QLength x{0_ft};
  okapi::QLength y{0_ft};

  /**
   * Default ctors and dtor
   */
  Point2D() = default;
  Point2D(const Point2D &point) = default;
  ~Point2D() = default;

  /**
   * Create a new Point2D
   * @param x X Value
   * @param y Y Value
   */
  Point2D(const okapi::QLength &x, const okapi::QLength &y);

  /**
   * Convert an OdomState to a point2D
   * @param state The OdomState
   */
  Point2D(const okapi::OdomState &state);

  /**
   * Indexing operators
   */
  okapi::QLength &operator[](size_t index);
  const okapi::QLength &operator[](size_t index) const;

  /**
   * Overload basic binary operators
   */
  Point2D operator+(const Point2D &rhs) const;
  Point2D operator-(const Point2D &rhs) const;
  Point2D operator*(const double rhs) const;
  Point2D operator/(const double rhs) const;

  /**
   * Overload comparison operators
   */
  bool operator==(const Point2D &rhs) const;
  bool operator!=(const Point2D &rhs) const;

  /**
   * Distance between 2 Point2D's
   */
  static okapi::QLength dist(const Point2D &lhs, const Point2D &rhs);
  okapi::QLength dist(const Point2D &point);

  /**
   * Distance to origin
   */
  static okapi::QLength magnitude(const Point2D &point);
  okapi::QLength magnitude();

  /**
   * Dot product of two vectors
   * lhs.x * rhs.x + lhs.y * rhs.y
   */
  static okapi::QArea dot(const Point2D &lhs, const Point2D &rhs);
  okapi::QArea dot(const Point2D &point);
};
