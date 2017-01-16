#include "MainLogic.hpp"

#include <cstdio>

#include "CVision.hpp"
#include "Gun.hpp"

#include "../stub_library/common.h" // TODO: Will be removed.

static int thold;

MainLogic::MainLogic() :
  drive_(Drive::getInstance()),
  sensor_(Sensor::getInstance()),
  gun_(Gun::getInstance()),
  vision_(CVision::getInstance())
{
}


MainLogic::~MainLogic()
{
}

void MainLogic::Init()
{
}

void MainLogic::Launch()
{
  // Get command from other interfaces.
  for (int i = 0; i < 1000; ++i) {
    int area, cx, cy;
    vision_.getInstance().ReadFrame();
    if (vision_.getInstance().DetectBlueBox(area, cx, cy)) {
      printf("area = %d, cx = %d, cy = %d\n", area, cx, cy);
      if (cx < -512) {
        drive_.TurnLeftPeriodInSeconds(0.5);
      }
      else if (cx > 512) {
        drive_.TurnRightPeriodInSeconds(0.5);
      }
      else {
        if (area < 3000) {
          drive_.RunForwardPeriodInSeconds(0.5);
        }
        if (area > 5000) {
          drive_.RunForwardPeriodInSeconds(-0.1);
        }
        else{
        //gun fire
        //gun_.open_fire(1);
        }
      }
    }
    int key = cv::waitKey(1) & 0xff;
    if (key == 27) {
      break;
    }
    printf("End loop\n");
  }
  cv::destroyAllWindows();
}
