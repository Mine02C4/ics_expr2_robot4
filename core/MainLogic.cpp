#include "MainLogic.hpp"

#include <cstdio>

#include "../stub_library/common.h" // TODO: Will be removed.

static int thold;

MainLogic::MainLogic() :
  drive_(Drive::getInstance()),
  sensor_(Sensor::getInstance())
{
}


MainLogic::~MainLogic()
{
}

void MainLogic::Init()
{
}

void MainLogic::Launch()
{
	// Get command from other interfaces.
  for (int i = 0; i < 100; ++i) {
    int rdist = Sensor::getInstance().GetDistance(SensorID::RightFront);
    int ldist = Sensor::getInstance().GetDistance(SensorID::LeftFront);
    printf("Distance RightFront = %d\n", rdist);
    printf("Distance LeftFront = %d\n", ldist);
    if (rdist - ldist > 10) {
      drive_.TurnLeftPeriodInSeconds(0.5);
    }
    else if (ldist - rdist > 10) {
      drive_.TurnRightPeriodInSeconds(0.5);
    }
    drive_.RunForwardPeriodInSeconds(1.5);
	printf("End loop\n");
  }
}
