#ifndef ROBOT_CORE_MAIN_LOGIC_H_
#define ROBOT_CORE_MAIN_LOGIC_H_

#include <thread>

#include "CVision.hpp"
#include "Drive.hpp"
#include "Sensor.hpp"
#include "Gun.hpp"
#include "../voice_recognition/voice_recog.hpp"
#include "../text_to_speech/text_to_speech.hpp"

enum class Mode {
  NoCompute,
  PursuingBox,
  ScanBox,
};

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
  std::thread camera_thread_;
  Mode mode_;
#ifndef _MSC_VER
  void Wait_Voice_By_Code();
  Voicerec &voice_;
  Speech &speech_;
#endif
  void AdjustGunTurret();
  void StartPursuingBox();
  void StartScanBox();
  void StartCameraLoop();
};

#endif  // ROBOT_CORE_MAIN_LOGIC_H_
