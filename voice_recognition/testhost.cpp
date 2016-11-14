#include <iostream>
#include <julius-simple.cpp>
int main(int argc, char const *argv[])
{
	register_callback(test);
	while(1) {}
	return 0;
}
void test(char * word) {
	printf("testok! %s¥n", word);
	return;
}