#include "trajectory2D.h"

Trajectory2D::Trajectory2D(const std::vector<Point2D> &path) : Path2D{path} {
}

Trajectory2D::Trajectory2D(std::vector<Point2D> &&path) : Path2D{path} {
}

std::vector<okapi::QCurvature> &Trajectory2D::getCurvatures() {
  return curvatures;
}

std::vector<okapi::QSpeed> &Trajectory2D::getVelocities() {
  return velocities;
}

Trajectory2D Trajectory2D::copy() const {
  Trajectory2D temp;
  temp.getPoints().reserve(points.size());
  for (auto &&point : points) {
    temp.getPoints().emplace_back(std::make_shared<Point2D>(*point));
  }

  temp.getCurvatures().reserve(curvatures.size());
  for (auto &&curvature : curvatures) {
    temp.getCurvatures().emplace_back(curvature);
  }

  temp.getVelocities().reserve(velocities.size());
  for (auto &&velocity : velocities) {
    temp.getVelocities().emplace_back(velocity);
  }

  return temp;
}

Trajectory2D &Trajectory2D::generateTrajectory(const okapi::QSpeed &maxVel,
                                               const okapi::QAcceleration &maxDecel,
                                               const okapi::QFrequency &kTurn) {
  setCurvatures();
  setVelocities(maxVel, maxDecel, kTurn);

  return *this;
}

Trajectory2D &Trajectory2D::withInterpolation(const okapi::QLength &resolution) {
  interpolate(resolution);
  return *this;
}

Trajectory2D &Trajectory2D::withSmoothening(double weight, int iterations) {
  smoothen(weight, iterations);
  return *this;
}

void Trajectory2D::setCurvatures() {
  curvatures.reserve(points.size());

  curvatures.emplace_back(0.0_mcrvt);
  for (size_t i = 1; i < points.size() - 1; i++) {
    okapi::QLength a = points[i]->dist(*points[i - 1]);
    okapi::QLength b = points[i + 1]->dist(*points[i]);
    okapi::QLength c = points[i - 1]->dist(*points[i + 1]);

    okapi::QVolume abc = a * b * c;
    okapi::QLength s = (a + b + c) / 2;
    auto area2 = s * (s - a) * (s - b) * (s - c);
    okapi::QArea area = area2.getValue() > 0.0 ? area2.sqrt() : 0.0_m * 0.0_m;

    okapi::QCurvature curvature = abc.getValue() ? 4 * area / abc : 0.0_mcrvt;

    curvatures.emplace_back(curvature);
  }
  curvatures.emplace_back(0.0_mcrvt);
}

void Trajectory2D::setVelocities(const okapi::QSpeed &maxVel,
                                 const okapi::QAcceleration &maxDecel,
                                 const okapi::QFrequency &kTurn) {
  velocities.resize(points.size());
  pros::delay(500);
  velocities.back() = 0_mps;
  for (int i = points.size() - 2; i >= 0; i--) {
    okapi::QSpeed targetVel = curvatures[i].getValue() ? kTurn / curvatures[i] : maxVel;
    okapi::QLength distance = points[i]->dist(*points[i + 1]);

    okapi::QSpeed decelLimited =
      (velocities[i + 1] * velocities[i + 1] + 2.0 * maxDecel * distance).sqrt();

    velocities[i] = std::min(std::min(targetVel, decelLimited), maxVel);
  }
}
