#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "videotest.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
  int mode;
  cout << "Select mode" << endl;
  cout << "1: Feature detection test" << endl;
  cout << "2: Calibration" << endl;
  cout << "3: Object detection by color" << endl;
  cin >> mode;
  switch (mode)
  {
  case 1:
  {
    VideoTest* vt = new VideoTest();
    vt->Init();

    while (1) {
      vt->ReadFrame();
      vt->TrackFeatures();
      int key = waitKey(1);
      if (key >= 0) {
        break;
      }
    }
    delete vt;
    break;
  }
  case 2:
    break;
  case 3:
  {
    VideoTest vt;
    vt.Init();
    while (1) {
      vt.ReadFrame();
      vt.DetectionByColor();
      int key = waitKey(1);
      if (key >= 0) {
        break;
      }
    }
    break;
  }
  default:
    break;
  }

  return 0;
}
