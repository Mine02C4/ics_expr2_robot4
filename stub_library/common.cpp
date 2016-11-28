#include "common.h"

#include <chrono>

#include "Simulator.h"

const double RotationRate = 45.0; // Degree per seconds

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
