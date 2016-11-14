#include <iostream>
#include "julius-simple.cpp"
void test(char *word);
int main(int argc, char *argv[])
{
	init_julius(argc, argv);
	register_callback(test);
	while(1) {}
	return 0;
}
void test(char * word) {
	printf("testok! %s¥n", word);
	return;
}