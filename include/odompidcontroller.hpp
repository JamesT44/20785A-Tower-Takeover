#pragma once

#include "customOdometry.h"
#include "main.h"

class OdomPIDController {
  public:
  /**
   * OdomController. Implements chassis movement algorithms
   *
   * @param imodel              The chassis model
   * @param iodometry           The chassis odometry
   * @param idistanceController The distance pid controller
   * @param iturnController     The turning pid controller
   * @param iangleController    The angle pid controller, used to keep
   * distance driving straight
   * @param idriveRadius        The radius from the target point to turn
   * off angle correction when driving to a point
   * @param itimeUtil           The time utility
   */
  OdomPIDController(
    const std::shared_ptr<okapi::ChassisModel> &model,
    const std::shared_ptr<CustomOdometry> &odometry,
    std::unique_ptr<okapi::IterativePosPIDController> distanceController,
    std::unique_ptr<okapi::IterativePosPIDController> turnController,
    std::unique_ptr<okapi::IterativePosPIDController> angleController,
    const okapi::TimeUtil &timeUtil);

  virtual ~OdomController() = default;

  /**
   * Turn the chassis to face an absolute angle
   *
   * @param angle   The angle
   * @param leftBias  How much to swing to the left [0-2] (1 = point turn)
   */
  void turnToAngle(const okapi::QAngle &angle, double leftBias = 1);

  /**
   * Turn the chassis to face a relative angle
   *
   * @param angle   The angle
   * @param leftBias  How much to swing to the left [0-2] (1 = point turn)
   */
  void turnAngle(const okapi::QAngle &angle, double leftBias = 1);

  /**
   * Drive a distance while maintaining starting angle
   *
   * @param distance The distance
   */
  void moveDistance(const okapi::QLength &distance);

  /**
   * Drives the robot straight to a point by turning to it first
   *
   * @param ipoint The target point to navigate to.
   */
  void driveToPoint(const okapi::Point &targetPoint);

  /**
   * Determines if the distance PID controller is settled.
   *
   * @return true if settled, false otherwise.
   */
  bool isDistanceSettled() const;

  /**
   * Determines if the angle PID controller is settled.
   *
   * @return true if settled, false otherwise.
   */
  bool isAngleSettled() const;

  /**
   * Determines if the turn PID controller is settled.
   *
   * @return true if settled, false otherwise.
   */
  bool isTurnSettled() const;

  protected:
  /**
   * Reset the pid controllers, used before every motion
   */
  virtual void resetPid();

  std::shared_ptr<okapi::ChassisModel> model{nullptr};
  const std::shared_ptr<CustomOdometry> odometry{nullptr};
  std::unique_ptr<IterativePosPIDController> distanceController{nullptr};
  std::unique_ptr<IterativePosPIDController> angleController{nullptr};
  std::unique_ptr<IterativePosPIDController> turnController{nullptr};
  okapi::TimeUtil timeUtil;

  okapi::QLength distanceErr = 0_in;
  okapi::QAngle angleErr = 0_deg;
}
