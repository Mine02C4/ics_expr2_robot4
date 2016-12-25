#include <iostream>
#include "text_to_speech.hpp"
int main() {
  Speech& sp = Speech::getInstance();
  sp.Speak("こんにちは");
  return 0;
}
