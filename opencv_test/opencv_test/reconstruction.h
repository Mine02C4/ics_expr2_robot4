#ifndef OPENCV_TEST_RECONSTRUCTION_H_
#define OPENCV_TEST_RECONSTRUCTION_H_

#include <vector>

class Reconstruction
{
public:
	Reconstruction();
	~Reconstruction();
	float GetX(int i);
	float GetY(int i);
	float GetZ(int i);
	void Add(float rowx, float rowy, float rowz, float theta);

private:
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> z;
};

#endif  // OPENCV_TEST_RECONSTRUCTION_H_
