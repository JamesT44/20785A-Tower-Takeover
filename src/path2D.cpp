#include "path2D.h"

Path2D::Path2D(const std::vector<Point2D> &path) {
  points.reserve(path.size());
  std::transform(path.begin(), path.end(), std::back_inserter(points), [](const auto &point) {
    return std::make_shared<Point2D>(point);
  });
}

Path2D::Path2D(std::vector<Point2D> &&path) {
  points.reserve(path.size());
  std::transform(path.begin(), path.end(), std::back_inserter(points), [](auto &&point) {
    return std::make_shared<Point2D>(std::move(point));
  });
}

const Path2D::points_t &Path2D::getPointsVector() const {
  return points;
}

Path2D::points_t &Path2D::pointsVector() {
  return points;
}

Path2D Path2D::copy() const {
  Path2D temp;
  temp.pointsVector().reserve(points.size());
  for (auto &&point : points) {
    temp.pointsVector().emplace_back(std::make_shared<Point2D>(*point));
  }
  return temp;
}

void Path2D::interpolate(const okapi::QLength &resolution) {
  points_t temp;
  temp.reserve(points.size());

  for (size_t i = 0; i < points.size() - 1; i++) {
    Point2D &start = *points[i];
    Point2D &end = *points[i + 1];

    Point2D diff = end - start;
    size_t steps = std::ceil((Point2D::magnitude(diff) / resolution).convert(okapi::number));
    Point2D step = diff / steps;
    temp.reserve(temp.size() + steps);
    for (size_t j = 0; j < steps; j++) {
      temp.emplace_back(std::make_shared<Point2D>(start + (step * j)));
    }
  }

  if (points.size() > 0) {
    temp.emplace_back(std::make_shared<Point2D>(*points.back()));
  }

  points = std::move(temp);
}

void Path2D::smoothen(double weight, int iterations) {

  double smoothWeight = 1.0 - weight;
  for (int k = 0; k < iterations; k++) {
    for (size_t i = 1; i < points.size() - 1; i++) {
      for (size_t j = 0; j < 2; j++) {
        okapi::QLength aux = (*points[i])[j];
        okapi::QLength dataFac = weight * ((*points[i])[j] - aux);
        okapi::QLength smoothFac =
          smoothWeight * ((*points[i - 1])[j] + (*points[i + 1])[j] - (2.0 * aux));
        (*points[i])[j] += dataFac + smoothFac;
      }
    }
  }
}

Path2D &Path2D::withInterpolation(const okapi::QLength &resolution) {
  interpolate(resolution);
  return *this;
}

Path2D &Path2D::withSmoothening(double weight, int iterations) {
  smoothen(weight, iterations);
  return *this;
}
