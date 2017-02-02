#include <iostream>
#include "text_to_speech.hpp"
int main() {
  Speech& sp = Speech::getInstance();
  sp.Sing("何言ってるのこの人_angry", 2);
  for (;;) {}
  return 0;
}
