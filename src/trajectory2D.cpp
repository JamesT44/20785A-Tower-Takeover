#include "trajectory2D.h"
#include <fstream>

Trajectory2D::Trajectory2D(const std::vector<Point2D> &path)
  : Path2D{path} {
}

Trajectory2D::Trajectory2D(std::vector<Point2D> &&path) : Path2D{path} {
}

const std::vector<okapi::QCurvature> &
Trajectory2D::getCurvaturesVector() const {
  return curvatures;
}

std::vector<okapi::QCurvature> &Trajectory2D::curvaturesVector() {
  return curvatures;
}

const std::vector<okapi::QSpeed> &
Trajectory2D::getVelocitiesVector() const {
  return velocities;
}

std::vector<okapi::QSpeed> &Trajectory2D::velocitiesVector() {
  return velocities;
}

Trajectory2D Trajectory2D::copy() const {
  Trajectory2D temp;
  temp.pointsVector().reserve(points.size());
  for (auto &&point : points) {
    temp.pointsVector().emplace_back(std::make_shared<Point2D>(*point));
  }

  temp.curvaturesVector().reserve(curvatures.size());
  for (auto &&curvature : curvatures) {
    temp.curvaturesVector().emplace_back(curvature);
  }

  temp.velocitiesVector().reserve(velocities.size());
  for (auto &&velocity : velocities) {
    temp.velocitiesVector().emplace_back(velocity);
  }

  return temp;
}

Trajectory2D &
Trajectory2D::generateTrajectory(const okapi::QSpeed &maxVel,
                                 const okapi::QAcceleration &maxDecel,
                                 const okapi::QFrequency &kTurn) {
  setCurvatures();
  setVelocities(maxVel, maxDecel, kTurn);

  return *this;
}

Trajectory2D &
Trajectory2D::withInterpolation(const okapi::QLength &resolution) {
  interpolate(resolution);
  return *this;
}

Trajectory2D &Trajectory2D::withSmoothening(double weight,
                                            int iterations) {
  smoothen(weight, iterations);
  return *this;
}

Trajectory2D::SDCardRes
Trajectory2D::saveToSD(const std::string &identifier) {
  if (!SDCardInserted()) {
    return Trajectory2D::SDCardRes::noSDCard;
  }

  std::ofstream f("/usd/paths/" + identifier + ".csv");
  if (!f.is_open()) {
    return Trajectory2D::SDCardRes::cannotOpenFile;
  }

  for (size_t i = 0; i < points.size(); i++) {
    f << points[i]->x.convert(okapi::meter) << ","
      << points[i]->y.convert(okapi::meter) << ","
      << curvatures[i].convert(okapi::mcrvt) << ","
      << velocities[i].convert(okapi::mps) << "\n";
  }
  f.close();

  return Trajectory2D::SDCardRes::success;
}

Trajectory2D::SDCardRes
Trajectory2D::loadFromSD(const std::string &identifier) {
  if (!SDCardInserted()) {
    return Trajectory2D::SDCardRes::noSDCard;
  }

  std::ifstream f("/usd/paths/" + identifier + ".csv");
  if (!f.is_open()) {
    return Trajectory2D::SDCardRes::cannotOpenFile;
  }

  std::string line, part;
  std::vector<std::string> parts;
  double x, y, curvature, velocity;
  while (!f.eof()) {
    getline(f, line);
    std::stringstream ss(line);

    while (getline(ss, part, ',')) {
      parts.emplace_back(part);
    }

    if (parts.size() == 0) {
      continue;
    }

    if (parts.size() != 4) {
      std::cout << parts.size() << std::endl;
      return Trajectory2D::SDCardRes::invalidDataFormat;
    }

    points.emplace_back(
      std::make_shared<Point2D>(std::stod(parts[0]) * okapi::meter,
                                std::stod(parts[1]) * okapi::meter));
    curvatures.emplace_back(std::stod(parts[2]) * okapi::mcrvt);
    velocities.emplace_back(std::stod(parts[3]) * okapi::mps);

    parts.clear();
  }
  f.close();

  return Trajectory2D::SDCardRes::success;
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
    okapi::QArea area =
      area2.getValue() > 0.0 ? area2.sqrt() : 0.0_m * 0.0_m;

    okapi::QCurvature curvature =
      abc.getValue() ? 4 * area / abc : 0.0_mcrvt;

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
    okapi::QSpeed targetVel =
      curvatures[i].getValue() ? kTurn / curvatures[i] : maxVel;
    okapi::QLength distance = points[i]->dist(*points[i + 1]);

    okapi::QSpeed decelLimited =
      (velocities[i + 1] * velocities[i + 1] + 2.0 * maxDecel * distance)
        .sqrt();

    velocities[i] = std::min(std::min(targetVel, decelLimited), maxVel);
  }
}

bool Trajectory2D::SDCardInserted() {
  std::ofstream f("/usd/sdcard_check");
  f.close();
  return f.fail();
}
