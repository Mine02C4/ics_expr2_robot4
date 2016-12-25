#ifndef TEXT_TO_SPEECH_H_
#define TEXT_TO_SPEECH_H_
#include <iostream>
#define NORMAL_FEEL "normal"

class Speech{
public:
  static Speech& getInstance(void) {
    static Speech singleton;
    return singleton;
  }
  void Init();
  void Finalize();
  int Speak(std::string);
  int Speak(std::string, std::string feeling);
private:
  Speech();
  int search_wav(std::string s, std::string feeling);
  int make_wav(std::string s, std::string feeling);
  int play_wav(std::string s, std::string feeling);
  //int rm_wav(std::string s, std:string feeling);
};
#endif // TEXT_TO_SPEECH_H_
