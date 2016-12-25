#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "text_to_speech.hpp"
#define VOICEDIR "./voices/"
const std::string dict = "/var/lib/mecab/dic/open-jtalk/naist-jdic/";
const std::string voicefiles = "~/mei/mei_normal.htsvoice";


int Speech::Speak(std::string s) {
  int res = Speech::Speak(s, NORMAL_FEEL);
  return res;
}
int Speech::Speak(std::string s, std::string feeling){
  if (search_wav(s, feeling) == -1) {
    if(make_wav(s, feeling) != -1) play_wav(s, feeling);
  } else {
    play_wav(s, feeling);
  }
  return 0;
}
int Speech::search_wav(std::string s, std::string feeling) {
  FILE * fp;
  std::string filename = (std::string)VOICEDIR + s + feeling;
  fp = fopen("filename", "r");
  if (fp == NULL) return -1; //not exist
  else return 0; //exist
}
int Speech::make_wav(std::string s, std::string feeling) {
  std::string com = "echo "+s+" | "+"open_jtalk -x "+dict+" -m "+voicefiles+" -ow "+(std::string)VOICEDIR+s+feeling+".wav";
  const char* cmd = com.c_str();
  system(cmd);
  return 0;
}
int Speech::play_wav(std::string s, std::string feeling) {
  std::string com = "aplay "+(std::string)VOICEDIR+s+feeling+".wav";
  const char* cmd = com.c_str();
  system(cmd);
  return 0;
}
void Speech::Init(){}
void Speech::Finalize(){}
Speech::Speech(){}
