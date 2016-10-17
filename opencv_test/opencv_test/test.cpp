#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "videotest.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
  Mat image;
  image = imread("yuiyui.jpg", IMREAD_COLOR); // Read the file

  if (!image.data) // Check for invalid input
  {
    cout << "Could not open or find the image" << std::endl;
    return -1;
  }

  namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
  imshow("Display window", image); // Show our image inside it.

  VideoTest* vt = new VideoTest();
  vt->Init();

  while (1) {
    vt->ReadFrame();
    int key = waitKey(1000);
    if (key >= 0) {
      break;
    }
  }

  delete vt;

  return 0;
}
