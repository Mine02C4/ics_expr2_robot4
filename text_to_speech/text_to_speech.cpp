#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "text_to_speech.hpp"
#include <thread>

#define VOICEDIR "./text_to_speech/voices/"
const std::string dict = "/var/lib/mecab/dic/open-jtalk/naist-jdic/";
//voice file places
const std::string normal_vf = "../mei/mei_normal.htsvoice";
const std::string angry_vf = "../mei/mei_angry.htsvoice";
const std::string happy_vf = "../mei/mei_happy.htsvoice";
const std::string sad_vf = "../mei/mei_sad.htsvoice";
const std::string bashful_vf = "../mei/mei_bashful.htsvoice";

int Speech::Speak(std::string s) {
  int res = Speech::Speak(s, NORMAL_FEEL);
  return res;
}

int Speech::Speak(std::string s, int feeling){
  if (search_wav(s, feeling) == -1) {
    if(make_wav(s, feeling) != -1) play_wav(s, feeling);
  } else {
    play_wav(s, feeling);
  }
  return 0;
}

int Speech::Speak_Through(std::string) {
  if (search_wav(s, NORMAL_FEEL) == -1) {
    if(make_wav(s, NORMAL_FEEL) != -1) play_wav_through(s, NORMAL_FEEL);
  } else {
    play_wav_through(s, NORMAL_FEEL);
  }
  return 0;
}

int Speech::search_wav(std::string s, int feeling) {
  FILE * fp;
  std::string filename = (std::string)VOICEDIR + s + convert_feel_to_string(feeling)+".wav";
  fp = fopen(filename.c_str(), "r");
  if (fp == NULL) return -1; //not exist
  else {
    fclose(fp);
    return 0;} //exist
}

int Speech::make_wav(std::string s, int feeling) {
  std::string voice_file;
  std::string voice_type = convert_feel_to_string(feeling);
  switch(feeling){
  case  NORMAL_FEEL:
    voice_file = normal_vf;
    break;
  case  ANGRY_FEEL:
    voice_file = angry_vf;
    break;
  case  HAPPY_FEEL:
    voice_file = happy_vf;
    break;
  case  SAD_FEEL:
    voice_file = sad_vf;
    break;
  case  BASHFUL_FEEL:
    voice_file = bashful_vf;
    break;
  default:
    voice_file = normal_vf;
    break;
  }
  std::string com = "echo "+s+" | "+"open_jtalk -x "+dict+" -m "+voice_file+" -ow "+(std::string)VOICEDIR+s+voice_type+".wav";
  const char* cmd = com.c_str();
  system(cmd);
  return 0;
}

int Speech::play_wav(std::string s, int feeling) {
  std::string voice_type = convert_feel_to_string(feeling);
  std::string com = "aplay -D plughw:0,0 "+(std::string)VOICEDIR+s+voice_type+".wav";
  const char* cmd = com.c_str();
  system(cmd);
  return 0;
}

int Speech::play_wav_through(std::string s, int feeling) {
  std::string voice_type = convert_feel_to_string(feeling);
  std::string com = "aplay -D plughw:0,0 "+(std::string)VOICEDIR+s+voice_type+".wav";
  const char* cmd = com.c_str();
  auto th = std::thread([this] {
    system(cmd);
    });
  th.detach();
  return 0;
}

std::string Speech::convert_feel_to_string(int feeling){
  std::string voice_type;
  switch(feeling){
  case  NORMAL_FEEL:
    voice_type = "_normal";
    break;
  case  ANGRY_FEEL:
    voice_type = "_angry";
    break;
  case  HAPPY_FEEL:
    voice_type = "_happy";
    break;
  case  SAD_FEEL:
    voice_type = "_sad";
    break;
  case  BASHFUL_FEEL:
    voice_type = "_bashful";
    break;
  default:
    voice_type = "_normal";
    break;
  }
  return voice_type;
}

void Speech::Init(){}
void Speech::Finalize(){}
Speech::Speech(){}
