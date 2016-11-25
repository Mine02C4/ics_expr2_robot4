#include "Drive.h"

#include "../motor_control/interface.h"

void Drive::Init()
{
  motor_init();
}

void Drive::RunForward(double seconds)
{
  run_forward(seconds);
}

Drive::Drive()
{
}

void Drive::Finalize()
{
  motor_finalize();
}
