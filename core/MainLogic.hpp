#ifndef ROBOT_CORE_MAIN_LOGIC_H_
#define ROBOT_CORE_MAIN_LOGIC_H_

#include <thread>

#include "CVision.hpp"
#include "Drive.hpp"
#include "Sensor.hpp"
#include "Gun.hpp"
#include "../voice_recognition/voice_recog.hpp"
#include "../text_to_speech/text_to_speech.hpp"

class MainLogic
{
public:
  MainLogic();
  ~MainLogic();
  void Init();
  void Launch();
  bool cv_task_flag_;
private:
  Drive &drive_;
  Sensor &sensor_;
  Gun &gun_;
  CVision &vision_;
  std::thread cv_thread_;
  void Wait_Input_By_Code();
#ifndef _MSC_VER
  Voicerec &voice_;
  Speech &speech_;
#endif
  void AdjustGunTurret();
  void StartPursuingBox();
};

#endif  // ROBOT_CORE_MAIN_LOGIC_H_
