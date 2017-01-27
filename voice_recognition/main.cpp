#include <iostream>
#include "voice_recog.hpp"
#include <string.h>
void test(std::string word);
int main(int argc, char *argv[])
{
  Voicerec &vc = Voicerec::getInstance();
  //  vc.Register_Callback(test);
  vc.Init();
  for (;;) {
  std::string s = vc.Wait_One_Sentence(5);
  std::cout << s << std::endl;
  }
  return 0;
}
void test(std::string word) {
  //  std::cout << word;
	return;
}
