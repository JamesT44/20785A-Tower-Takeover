#pragma once

class PathGenerator {
public:
  static PursuitPath generate(const Path& ipath);

protected:
  static void setCurvatures(PursuitPath& ipath);

  static void setMaxVelocity(PursuitPath& ipath, const PursuitLimits& limits);

  static double calculateCurvature(const Vector& prev, const Vector& point, const Vector& next);
};
