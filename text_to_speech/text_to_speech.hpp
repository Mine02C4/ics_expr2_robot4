#ifndef TEXT_TO_SPEECH_H_
#define TEXT_TO_SPEECH_H_

#include <iostream>
#include <thread>
#include <utility>
//define feelings
#define NORMAL_FEEL 0
#define ANGRY_FEEL 1
#define HAPPY_FEEL 2
#define SAD_FEEL 3
#define BASHFUL_FEEL 4

#define SPEAKTHROUGH 5

class Speech{
public:
  static Speech& getInstance(void) {
    static Speech singleton;
    return singleton;
  }
  void Init();
  void Finalize();
  int Speak(std::string);
  int Speak(std::string, int feeling);
  int Speak_Through(std::string s);
  int Sing(std::string s, int second);
private:
  Speech();
  std::string convert_feel_to_string(int feeling);
  int search_wav(std::string , int  feeling);
  int make_wav(std::string s, int  feeling);
  int play_wav(std::string s, int  feeling);
  int play_wav_through(std::string s, int feeling);
  int play_wav_through_second(std::string s, int feeling, int second);
  //int rm_wav(std::string s, int feeling);
};

#endif // TEXT_TO_SPEECH_H_
