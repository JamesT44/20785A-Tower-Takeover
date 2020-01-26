#include "point2D.h"
#include "main.h"
#include "okapi/api/units/RQuantity.hpp"
#include <stdexcept>

Point2D::Point2D(const okapi::QLength &x, const okapi::QLength &y)
  : x(x), y(y) {
}

Point2D::Point2D(const okapi::OdomState &state) : x(state.x), y(state.y) {
}

okapi::QLength &Point2D::operator[](size_t index) {
  switch (index) {
  case 0:
    return x;
    break;

  case 1:
    return y;
    break;

  default:
    throw std::runtime_error("Vector[]: \"" + std::to_string(index) +
                             "\" is invalid index");
    break;
  }
}

const okapi::QLength &Point2D::operator[](size_t index) const {
  return const_cast<Point2D *>(this)->operator[](index);
}

Point2D Point2D::operator+(const Point2D &rhs) const {
  return {x + rhs.x, y + rhs.y};
}

Point2D Point2D::operator-(const Point2D &rhs) const {
  return {x - rhs.x, y - rhs.y};
}

bool Point2D::operator==(const Point2D &rhs) const {
  return x == rhs.x && y == rhs.y;
}

bool Point2D::operator!=(const Point2D &rhs) const {
  return !(rhs == *this);
}

Point2D Point2D::operator*(const double scalar) const {
  return {x * scalar, y * scalar};
}

Point2D Point2D::operator/(const double scalar) const {
  return {x / scalar, y / scalar};
}

okapi::QLength Point2D::dist(const Point2D &lhs, const Point2D &rhs) {
  return ((lhs.x - rhs.x) * (lhs.x - rhs.x) +
          (lhs.y - rhs.y) * (lhs.y - rhs.y))
    .sqrt();
}

okapi::QLength Point2D::dist(const Point2D &point) {
  return dist(*this, point);
}

okapi::QLength Point2D::magnitude(const Point2D &point) {
  return dist(point, {0_ft, 0_ft});
}

okapi::QLength Point2D::magnitude() {
  return magnitude(*this);
}

okapi::QArea Point2D::dot(const Point2D &lhs, const Point2D &rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

okapi::QArea Point2D::dot(const Point2D &point) {
  return dot(*this, point);
}
