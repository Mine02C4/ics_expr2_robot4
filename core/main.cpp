#include <cstdio>

#include "Drive.h"
#include "MainLogic.h"
#include "Sensor.h"

int main()
{
  printf("Start!\n");
  Drive::getInstance().Init();
  Sensor::getInstance().Init();
  Drive::getInstance().Init();
  printf("Finish init devices.\n");
  MainLogic logic;
  logic.Init();
  printf("Finish init mainlogic.\n");
  logic.Launch();
  Sensor::getInstance().Finalize();
  Drive::getInstance().Finalize();
  return 0;
}

