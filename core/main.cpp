#include <cstdio>

#include "Drive.hpp"
#include "MainLogic.hpp"
#include "Sensor.hpp"
#include "../voice_recognition/voice_recog.hpp"
#include "../text_to_speech/text_to_speech.hpp"

int main()
{
  printf("Start!\n");
  Drive::getInstance().Init();
  Sensor::getInstance().Init();
//  Gun::getInstance().Init();
  CVision::getInstance().Init();
  Voicerec::getInstance().Init();
  Speech::getInstance().Init();
  printf("Finish init devices.\n");
  MainLogic logic;
  logic.Init();
  printf("Finish init mainlogic.\n");
  logic.Launch();
  Voicerec::getInstance().Finalize();
 // Gun::getInstance().Finalize();
  Sensor::getInstance().Finalize();
  Drive::getInstance().Finalize();
  return 0;
}

