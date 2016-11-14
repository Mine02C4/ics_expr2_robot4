#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "videotest.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
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

  return 0;
}
