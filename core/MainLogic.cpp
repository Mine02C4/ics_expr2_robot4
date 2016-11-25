#include "MainLogic.h"

#include <cstdio>

MainLogic::MainLogic() :
  Sensor(Sensor::getInstance()),
  Drive(Drive::getInstance())
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
  Drive.RunForward(1.5);
}
