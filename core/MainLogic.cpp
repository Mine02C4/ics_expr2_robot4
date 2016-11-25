#include "MainLogic.h"

#include <cstdio>

#include "../stub_library/common.h" // TODO: Will be removed.

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
  for (int i = 0; i < 100; ++i) {
    int rdist = Sensor::getInstance().GetDistance(SensorID::RightFront);
    int ldist = Sensor::getInstance().GetDistance(SensorID::LeftFront);
    printf("Distance RightFront = %d\n", rdist);
    printf("Distance LeftFront = %d\n", ldist);
    if (rdist - ldist > 5) {
      drive_.TurnLeftPeriodInSeconds(1);
    }
    else if (ldist - rdist > 5) {
      drive_.TurnRightPeriodInSeconds(1);
    }
    else {
      sleep_seconds(1);
    }
  }
}
