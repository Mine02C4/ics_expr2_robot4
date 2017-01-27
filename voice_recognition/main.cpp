#include <iostream>
#include "voice_recog.hpp"
#include <string.h>
void test(std::string word);
int main(int argc, char *argv[])
{
  Voicerec &vc = Voicerec::getInstance();
  //  vc.Register_Callback(test);
  vc.Init();
  int s = vc.Wait_One_Code(10);
  std::cout << s;
  return 0;
}
void test(std::string word) {
  //  std::cout << word;
	return;
}
