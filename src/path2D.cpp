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

std::vector<std::shared_ptr<Point2D>> &Path2D::getVector() {
  return points;
}

Path2D Path2D::copy() const {
  Path2D temp;
  temp.getVector().reserve(points.size());
  for (auto &&point : points) {
    temp.getVector().emplace_back(std::make_shared<Point2D>(*point));
  }
  return temp;
}

Path2D &Path2D::interpolate(const okapi::QLength &resolution) {
  std::vector<std::shared_ptr<Point2D>> temp;

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
  return *this;
}

Path2D &Path2D::smoothen(double weight, const okapi::QLength &tolerance) {
  Path2D temp = copy();
  double smoothWeight = 1.0 - weight;
  auto change = tolerance;
  while (change >= tolerance) {
    change = 0.0_m;
    for (size_t i = 1; i < points.size() - 1; i++) {
      for (size_t j = 0; j < 2; j++) {
        okapi::QLength aux = (*temp.getVector()[i])[j];
        okapi::QLength dataFac = weight * ((*points[i])[j] - aux);
        okapi::QLength smoothFac = smoothWeight * ((*temp.getVector()[i - 1])[j] +
                                                   (*temp.getVector()[i + 1])[j] - (2.0 * aux));
        (*temp.getVector()[i])[j] += dataFac + smoothFac;
        change = (aux - (*temp.getVector()[i])[j]).abs();
      }
    }
  }

  points = std::move(temp.getVector());
  return *this;
}
