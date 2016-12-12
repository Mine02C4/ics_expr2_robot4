#include <cstdio>

#include "Drive.hpp"
#include "MainLogic.hpp"
#include "Sensor.hpp"

int main()
{
  printf("Start!\n");
  Drive::getInstance().Init();
  Sensor::getInstance().Init();
  printf("Finish init devices.\n");
  MainLogic logic;
  logic.Init();
  printf("Finish init mainlogic.\n");
  logic.Launch();
  Sensor::getInstance().Finalize();
  Drive::getInstance().Finalize();
  return 0;
}

