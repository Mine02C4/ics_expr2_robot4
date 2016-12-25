#include <iostream>
#include "voice_recog.hpp"
void test(std::string word);
int main(int argc, char *argv[])
{
  Voicerec &vc = Voicerec::getInstance();
  vc.Register_Callback(test);
  vc.Init();
  return 0;
}
void test(std::string word) {
  std::cout << word;
	return;
}
