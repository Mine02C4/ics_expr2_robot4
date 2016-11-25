#include "MainLogic.h"

#include <cstdio>

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
  int dist = Sensor::getInstance().GetDistance(SensorID::RightFront);
  printf("Distance S1 d = %d\n", dist);
  drive_.RunForward(1.5);
}
