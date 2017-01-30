#include "Drive.hpp"
#include <iostream>

#include "../motor_control/interface.h"
#define PI 3.14159265
#define CONV_TO_DIST 10

void Drive::Init()
{
  std::cout << "Initializing motor, moving to inital pos...";
  motor_init();
	std::cout << "complete!\n"
}

void Drive::SetCurrentPosition(int cval[2])
{
	/* TODO: get current positon from cval */
	/* TODO: set private in class */
	double dsl, dsr, ds;
	const double R = 500;	// diamiter	[mm]
	double dx, dy;		// [mm]
	double dth;		// diff theta
	double th;		// current theta [deg]
	double x, y;

	dsl = cval[0] * CONV_TO_DIST;		// LEFT
	dsr = cval[1] * CONV_TO_DIST;		// RIGHT

	ds = (dsr + dsl) / 2;
	dth = ((dsr - dsl) / R);
	dx = ds * cos((th + (dth / 2)) * (PI / 180.0));
	dy = ds * sin((th + (dth / 2)) * (PI / 180.0));
	
	x = x + dx;
	y = y + dy;
	th = th + dth;
	std::cout << "Current Pos [x, y, th] = " << x << y << th;

}

void Drive::RunForward(double rot)
{
	/* TODO: check cval */
  static int currentrot;
  std::cout << "currentrot:" << currentrot;
  currentrot += rot;
  run_forward(rot);
  return;
}

void Drive::Turn(double angle)
{
	static int current angle;
}

void Drive::TurnRight(double angle)
{
  turn_right(angle);
  return;
}

void Drive::TurnLeft(double angle)
{
  static int currangle;
  turn_left(angle);
  return;
}

void Drive::SetDesire(double rot[2], double speed[2])
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
