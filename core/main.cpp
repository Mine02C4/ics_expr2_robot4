#include <cstdio>

#include "Drive.h"
#include "MainLogic.h"
#include "Sensor.h"

int main()
{
  printf("Start!\n");
  Drive::getInstance().Init();
  Sensor::getInstance().Init();
  MainLogic logic;
  logic.Init();
  logic.Launch();
  Sensor::getInstance().Finalize();
  Drive::getInstance().Finalize();
  return 0;
}

