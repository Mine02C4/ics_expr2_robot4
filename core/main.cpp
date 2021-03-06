#include <cstdio>

#include <string>

#include "Drive.hpp"
#include "MainLogic.hpp"
#include "Sensor.hpp"
#ifndef _MSC_VER
#include "../voice_recognition/voice_recog.hpp"
#include "../text_to_speech/text_to_speech.hpp"
#endif

int main(int argc, char * argv[])
{
  bool nowindow = false;
  if (argc > 1) {
    std::string arg1 = argv[1];
    if (arg1 == "nowindow") {
      nowindow = true;
    }
  }
  printf("Start!\n");
  Drive::getInstance().Init();
  Sensor::getInstance().Init();
  Gun::getInstance().Init();
  CVision::getInstance().Init(nowindow);
#ifndef _MSC_VER
  Voicerec::getInstance().Init();
  Speech::getInstance().Init();
#endif
  printf("Finish init devices.\n");
  MainLogic logic;
  logic.Init();
  printf("Finish init mainlogic.\n");
  logic.Launch();
#ifndef _MSC_VER
  Voicerec::getInstance().Finalize();
#endif
  Gun::getInstance().Finalize();
  Sensor::getInstance().Finalize();
  Drive::getInstance().Finalize();
  return 0;
}

