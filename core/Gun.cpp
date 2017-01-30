#include "Gun.hpp"

#include <iostream>
#include <thread>

#include "../gun_turret/interface.h"

void Gun::Init()
{
  turret_init();
  current_elevation_ = 0;
}

void Gun::Finalize()
{
  turret_finalize();
}

void Gun::FireSingle()
{
  open_fire(1);
}

void Gun::FireNum(int numbullets)
{
  open_fire(numbullets);
}

void Gun::FireBurst(int ntimes)
{
  burst_fire(ntimes);
}

void Gun::TurretAbsoluteElevate(int degrees)
{
  if (degrees < kElevationLowerLimit) {
    std::cerr << "Core Gun: Elevate too lower " << degrees << std::endl;
    degrees = kElevationLowerLimit;
  }
  else if (degrees > kElevationUpperLimit) {
    std::cerr << "Core Gun: Elevate too upper " << degrees << std::endl;
    degrees = kElevationUpperLimit;
  }
  elevate_by_degrees(degrees);
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Gun::TurretRelativeUp(int degrees)
{
  TurretAbsoluteElevate(current_elevation_ + degrees);
}

Gun::Gun()
{
}
