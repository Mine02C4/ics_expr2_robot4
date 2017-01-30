#include "Drive.hpp"
#include <iostream>
#include <math.h>

#include "../motor_control/interface.h"
#define PI 3.14159265
#define CONV_TO_DIST 1.43333
#define CONV_TO_MTR  0.69767
#define MOT_R 430	

void Drive::Init()
{
  std::cout << "Initializing motor, moving to inital pos...";
  motor_init();
	std::cout << "complete!\n";
}

void Drive::SetCurrentPosition(int val[2])
{
	/* TODO: get current positon from cval */
	/* TODO: set private in class */
	double dsl, dsr, ds;
	const double R = 430;	// diamiter	[mm]
	double dx, dy;		// [mm]
	double dth;		// diff theta
  double x = cval[0];
  double y = cval[1];
	double th = cval[2];		// current theta [deg]

	std::cout << "Old Pos [x, y, th] = " << x << y << th;
	
  dsl = val[0] * CONV_TO_DIST;		// LEFT
	dsr = val[1] * CONV_TO_DIST;		// RIGHT

	ds = (dsr + dsl) / 2;
	dth = ((dsr - dsl) / R);
	dx = ds * cos((th + (dth / 2)) * (PI / 180.0));
	dy = ds * sin((th + (dth / 2)) * (PI / 180.0));
	
	x = x + dx;
	y = y + dy;
	th = th + dth;
	std::cout << "Current Pos [x, y, th] = " << x << y << th;
	cval[0] = x;
	cval[1] = y;
	cval[2] = th;
}

void Drive::RunForward(double rot)
{
	/* TODO: check cval */
  int rotv[2] = {static_cast<int>(rot * CONV_TO_MTR), 
  static_cast<int>(rot * CONV_TO_MTR)};
  static int currentrot;
  std::cout << "currentrot:" << currentrot;
  SetCurrentPosition(rotv);
  currentrot += rot;
  run_forward(rot);
  return;
}

void Drive::Turn(double angle)
{
	if (angle > 0.0) {
		TurnLeft(angle);
	} else if (angle < 0.0) {
		TurnRight(-angle);
	}
	return;
}

void Drive::TurnRight(double angle)
{
	int mot = (int) angle * MOT_R;
	int motv[2] = {mot, 0};
  turn_right(mot);
	SetCurrentPosition(motv);
  return;
}

void Drive::TurnLeft(double angle)
{
	int mot = (int) angle * MOT_R;
	int motv[2] = {mot, 0};
	turn_left(mot);
	SetCurrentPosition(motv);
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
