#include "Drive.h"

#include "../motor_control/interface.h"

void Drive::Init()
{
  motor_init();
}

void Drive::RunForwardPeriodInSeconds(double seconds)
{
  run_forward(seconds);
}

void Drive::TurnRightPeriodInSeconds(double seconds)
{
  turn_right(seconds);
}

void Drive::TurnLeftPeriodInSeconds(double seconds)
{
  turn_left(seconds);
}

Drive::Drive()
{
}

void Drive::Finalize()
{
  motor_finalize();
}
