#ifndef ROBOT_CORE_MAIN_LOGIC_H_
#define ROBOT_CORE_MAIN_LOGIC_H_

#include "Drive.h"
#include "Sensor.h"

class MainLogic
{
public:
  MainLogic();
  ~MainLogic();
  void Init();
  void Launch();
private:
  Drive &Drive;
  Sensor &Sensor;
};

#endif  // ROBOT_CORE_MAIN_LOGIC_H_
