#include <iostream>
#include "julius-simple.h"
void test(std::string word);
int main(int argc, char *argv[])
{
  	register_callback(test);
	init_julius(argc, argv);
	return 0;
}
void test(std::string word) {
  std::cout << word;
	return;
}
