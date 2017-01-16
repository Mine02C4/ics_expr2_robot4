#include "Drive.hpp"

#include "../motor_control/interface.h"

void Drive::Init()
{
	std::cout "Initializing motor, moving to inital pos\n";
	run_forward(0);
  motor_init();
}

void Drive::RunForward(double rot)
{
	run_forward(rot);
	return;
}

void Drive::TurnRight(double angle)
{
	turn_right(angle);
	return;
}

void Drive::TurnLeft(double angle)
{
	turn_left(angle);
	return;
}

void Drive::SetDesire(double rot[], double speed[])
{
	// TODO: implement
	return;
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
