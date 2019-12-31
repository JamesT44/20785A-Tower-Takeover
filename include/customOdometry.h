#pragma once

#include "main.h"
#include <atomic>
#include <memory>
#include <valarray>

class CustomOdometry : public okapi::Odometry {
  public:
  /**
   * CustomOdometry. Tracks the movement of the robot and estimates its position in coordinates
   * relative to the start (assumed to be (0, 0, 0)).
   *
   * @param itimeUtil The TimeUtil.
   * @param imodel The chassis model for reading sensors.
   * @param ichassisScales The chassis dimensions.
   * @param ilogger The logger this instance will log to.
   */
  CustomOdometry(const okapi::TimeUtil &itimeUtil,
                 const std::shared_ptr<okapi::ReadOnlyChassisModel> &imodel,
                 const okapi::ChassisScales &ichassisScales,
                 const std::shared_ptr<okapi::Logger> &ilogger = okapi::Logger::getDefaultLogger());

  virtual ~CustomOdometry() = default;

  /**
   * Sets the drive and turn scales.
   */
  void setScales(const okapi::ChassisScales &ichassisScales) override;

  /**
   * Do one odometry step.
   */
  void step() override;

  /**
   * Returns the current state.
   *
   * @param imode The mode to return the state in.
   * @return The current state in the given format.
   */
  okapi::OdomState
  getState(const okapi::StateMode &imode = okapi::StateMode::FRAME_TRANSFORMATION) const override;

  /**
   * Sets a new state to be the current state.
   *
   * @param istate The new state in the given format.
   * @param imode The mode to treat the input state as.
   */
  void setState(const okapi::OdomState &istate,
                const okapi::StateMode &imode = okapi::StateMode::FRAME_TRANSFORMATION) override;

  /**
   * @return The internal ChassisModel.
   */
  std::shared_ptr<okapi::ReadOnlyChassisModel> getModel() override;

  /**
   * @return The internal ChassisScales.
   */
  okapi::ChassisScales getScales() override;

  protected:
  std::shared_ptr<okapi::Logger> logger;
  std::unique_ptr<okapi::AbstractRate> rate;
  std::unique_ptr<okapi::AbstractTimer> timer;
  std::shared_ptr<okapi::ReadOnlyChassisModel> model;
  okapi::ChassisScales chassisScales;
  okapi::OdomState state;
  std::valarray<std::int32_t> newTicks{0, 0, 0}, tickDiff{0, 0, 0}, lastTicks{0, 0, 0};
  const std::int32_t maximumTickDiff{1000};

  /**
   * Does the math, side-effect free, for one odom step.
   *
   * @param itickDiff The tick difference from the previous step to this step.
   * @param ideltaT The time difference from the previous step to this step.
   * @return The newly computed OdomState.
   */
  virtual okapi::OdomState odomMathStep(const std::valarray<std::int32_t> &itickDiff,
                                        const okapi::QTime &ideltaT);
};
