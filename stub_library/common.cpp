#define _USE_MATH_DEFINES
#include "common.h"

#include <cmath>  

#include <chrono>

#include "Simulator.h"

const double RotationRate = 45.0; // Degree per seconds
const double WRotationPerSeconds = 0.8;
const double WLengthPerRotation = 10.0;

#define SIM(sim) stub::Simulator &sim = stub::Simulator::getInstance()

void AnimateSimulator(double duration, std::function<void(double)> func)
{
  auto start = std::chrono::system_clock::now();
  double elapsed;
  do {
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
      (std::chrono::system_clock::now() - start).count() / 1000.0;
    func(std::min(elapsed, duration));
    sleep_seconds(0.01);
  } while (elapsed < duration);
  func(duration);
}

void RunForwardViaNumberOfRotations(double num) {
  SIM(sim);
  double init_x = sim.x;
  double init_y = sim.y;
  AnimateSimulator(num / WRotationPerSeconds, [&sim, init_x, init_y](double elapsed) {
    std::lock_guard<std::mutex> lock(sim.mtx_);
    auto rad = sim.direction_ * M_PI / 180.0;
    sim.x = init_x - sin(rad) * WLengthPerRotation * WRotationPerSeconds * elapsed;
    sim.y = init_y - cos(rad) * WLengthPerRotation * WRotationPerSeconds * elapsed;
  });
}

void TurnRightPeriodInSeconds(double seconds)
{
  SIM(sim);
  double init_dir = sim.direction_;
  AnimateSimulator(seconds, [&sim, init_dir](double elapsed) {
    std::lock_guard<std::mutex> lock(sim.mtx_);
    sim.direction_ = init_dir - elapsed * RotationRate;
  });
}

void TurnLeftPeriodInSeconds(double seconds)
{
  SIM(sim);
  double init_dir = sim.direction_;
  AnimateSimulator(seconds, [&sim, init_dir](double elapsed) {
    std::lock_guard<std::mutex> lock(sim.mtx_);
    sim.direction_ = init_dir + elapsed * RotationRate;
  });
}

double GetDistance(SensorPosition sensor)
{
  SIM(sim);
  switch (sensor)
  {
  case LEFT_FRONT:
    return sim.GetDistance(0);
  case RIGHT_FRONT:
    return sim.GetDistance(1);
  default:
    return -1.0;
  }
}
