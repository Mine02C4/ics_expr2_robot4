#include <iostream>
#include "text_to_speech.hpp"
int main() {
  Speech& sp = Speech::getInstance();
  sp.Speak("何言ってるのこの人",ANGRY_FEEL);
  return 0;
}
