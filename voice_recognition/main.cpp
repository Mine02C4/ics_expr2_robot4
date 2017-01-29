#include <iostream>
#include "voice_recog.hpp"
#include <string.h>
void test(std::string word);
int main(int argc, char *argv[])
{
  Voicerec &vc = Voicerec::getInstance();
  std::cout << "first" << std::endl;
  // vc.Register_Callback(test);
  vc.Init();
  std::cout << "ok" << std::endl;
  std::cout << vc.Wait_One_Sentence(10);
  return 0;
}
void test(std::string word) {
  std::cout << word;
	return;
}
