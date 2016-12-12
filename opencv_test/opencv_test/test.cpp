#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
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
  {
	  const int BOARD_W = 11; //‰¡
	  const int BOARD_H = 6;
	  const Size BOARD_SIZE = Size(BOARD_W, BOARD_H);
	  const int N_CORNERS = BOARD_H * BOARD_W;
	  const int N_BORDARS = 2;

	  break;
  }    
  case 3:
    break;
  default:
    break;
  }

  return 0;
}
