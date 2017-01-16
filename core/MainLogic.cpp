#include "MainLogic.hpp"

#include <cstdio>

#include <string>

#include "CVision.hpp"

#include "../stub_library/common.h" // TODO: Will be removed.

static int thold;

MainLogic::MainLogic() :
  drive_(Drive::getInstance()),
  sensor_(Sensor::getInstance()),
  gun_(Gun::getInstance()),
  vision_(CVision::getInstance()),
  voice_(Voicerec::getInstance())
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
				drive_.TurnLeft(3);
//        drive_.TurnLeftPeriodInSeconds(0.5);
      }
      else if (cx > 512) {
				drive_.TurnRight(3);
//        drive_.TurnRightPeriodInSeconds(0.5);
      }
      else {
        if (area < 3000) {
					drive_.RunForward(30);
//          drive_.RunForwardPeriodInSeconds(0.5);
        }
        if (area > 5000) {
					drive_.RunForward(-30);
//          drive_.RunForwardPeriodInSeconds(-0.5);
        }
        else{
        //gun adjustment
          if (cy < -512) {
            gun_.TurretUp();
            printf("turretup");
          }
          else if (cy > 512) {
            gun_.TurretDown();
            printf("turretdown");
          }
        }
      }
    }
    std::string str = voice_.Wait_One_Sentence(5);
    if (str != "") {
      printf("%s\n", str.c_str());
    }
    int key = cv::waitKey(1) & 0xff;
    if (key == 27) {
      break;
    }
    printf("End loop\n");
  }
  cv::destroyAllWindows();
}
