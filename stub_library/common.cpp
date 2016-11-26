#include "common.h"

#include <chrono>

#include "Simulator.h"

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
  stub::Simulator &sim = stub::Simulator::getInstance();
  double init_dir = sim.direction_;
  AnimateSimulator(seconds, [&sim, init_dir](double elapsed) {
    std::lock_guard<std::mutex> lock(sim.mtx_);
    sim.direction_ = init_dir - elapsed * RotationRate;
  });
}

void TurnLeftPeriodInSeconds(double seconds)
{
  stub::Simulator &sim = stub::Simulator::getInstance();
  double init_dir = sim.direction_;
  AnimateSimulator(seconds, [&sim, init_dir](double elapsed) {
    std::lock_guard<std::mutex> lock(sim.mtx_);
    sim.direction_ = init_dir + elapsed * RotationRate;
  });
}
