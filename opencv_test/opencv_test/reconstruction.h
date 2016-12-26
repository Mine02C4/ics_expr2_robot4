#ifndef OPENCV_TEST_RECONSTRUCTION_H_
#define OPENCV_TEST_RECONSTRUCTION_H_

#include <vector>
#include <opencv2/core/core.hpp>

class Reconstruction
{
public:
	Reconstruction();
	~Reconstruction();
	float GetX(int i);
	float GetY(int i);
	float GetZ(int i);
	void Add(float rowx, float rowy, float rowz, float theta);
  void AddFrom3D(cv::Mat image);

private:
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> z;
};

#endif  // OPENCV_TEST_RECONSTRUCTION_H_
