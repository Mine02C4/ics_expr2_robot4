#include <iostream>
#include "voice_recog.hpp"
#include "voicecode.hpp"
#include <string.h>
void test(std::string word);
int main(int argc, char *argv[])
{
  Voicerec &vc = Voicerec::getInstance();
  //  vc.Register_Callback(test);
  vc.Init();
  struct voicecode vcs;
  for (;;) {
    vc.Wait_One_Code(5, vcs);
    std::string s = vc.getString();
    std::cout << "string is" << s  << "code:"<< vcs.code << "num:" << vcs.num <<   std::endl;
  }
  return 0;
}
void test(std::string word) {
  //  std::cout << word;
	return;
}
