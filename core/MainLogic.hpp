#ifndef ROBOT_CORE_MAIN_LOGIC_H_
#define ROBOT_CORE_MAIN_LOGIC_H_

#include "CVision.hpp"
#include "Drive.hpp"
#include "Sensor.hpp"
#include "Gun.hpp"
#include "../voice_recognition/voice_recog.hpp"

class MainLogic
{
public:
  MainLogic();
  ~MainLogic();
  void Init();
  void Launch();
private:
  Drive &drive_;
  Sensor &sensor_;
  Gun &gun_;
  CVision &vision_;
  Voicerec &voice_;
};

#endif  // ROBOT_CORE_MAIN_LOGIC_H_
