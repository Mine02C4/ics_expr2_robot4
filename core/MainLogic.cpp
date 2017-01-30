#include "MainLogic.hpp"

#include <cstdio>

#include <string>

#include "CVision.hpp"

#include "../stub_library/common.h" // TODO: Will be removed.

#include "../voice_recognition/voicecode.hpp"

static int thold;

MainLogic::MainLogic() :
  drive_(Drive::getInstance()),
  sensor_(Sensor::getInstance()),
  gun_(Gun::getInstance()),
  vision_(CVision::getInstance())
#ifndef _MSC_VER
  , voice_(Voicerec::getInstance()),
  speech_(Speech::getInstance())
#endif
{
  cv_task_flag_ = false;
}


MainLogic::~MainLogic()
{
}

void MainLogic::Init()
{
}

void MainLogic::AdjustGunTurret()
{
  int area, cx, cy;
  if (vision_.getInstance().DetectBlueBox(area, cx, cy)) {
    printf("area = %d, cx = %d, cy = %d\n", area, cx, cy);
    if (cx < -512) {
    }
    else if (cx > 512) {
    }
    else {
      if (area < 3000) {
        drive_.RunForward(1);
      }
      if (area > 5000) {
        drive_.RunForward(-1);
      }
      else {
        //gun adjustment
        if (cy < -512) {
          gun_.TurretRelativeUp(1 - cy / 10);
          printf("turretup");
        }
        else if (cy > 512) {
          gun_.TurretRelativeUp(-1 - cy / 10);
          printf("turretdown");
        }
      }
    }
  }
}

void MainLogic::StartPursuingBox()
{
  cv_thread_ = std::thread([this] {
    while (cv_task_flag_) {
      if (vision_.getInstance().ReadFrame()) {
        AdjustGunTurret();
      }
      int key = cv::waitKey(1) & 0xff;
      if (key == 27) {
        break;
      }
    }
  });
}

void MainLogic::Launch()
{
  // Get command from other interfaces.
  cv_task_flag_ = true;
  StartPursuingBox();
  for (int i = 0; i < 1000; ++i) {
    int leftfront, rightfront;
    leftfront = sensor_.GetDistance(SensorID::LeftFront);
    rightfront = sensor_.GetDistance(SensorID::RightFront);
    if (leftfront < 50 || rightfront < 50) {
      printf("stop\n");
      drive_.RunForward(0);
    }
    printf("get_distance(left) : %d\n", sensor_.GetDistance(SensorID::LeftFront));
    printf("get_distance(right) : %d\n", sensor_.GetDistance(SensorID::RightFront));
#ifndef _MSC_VER
    //  std::string str = voice_.Wait_One_Sentence(5);
    int code = voice_.Wait_One_Code(5);
    printf("code:%d\n", code);
    switch (code) {
    case VC_CODE_FIRE:
      printf("!FIRE\n");
      break;
    case VC_CODE_FORWARD:
      printf("RunForward\n");
      drive_.RunForward(200);
      break;
    case VC_CODE_BACK:
      printf("RunBack\n");
      drive_.RunForward(-200);
      break;
    case VC_CODE_LEFT:
      printf("!Left\n");
      break;
    case VC_CODE_RIGHT:
      printf("!Right\n");
      break;
    case VC_CODE_ROTATE:
      printf("!Rotate\n");
      break;
    case VC_CODE_STOP:
      printf("!STOP\n");
      break;
    default:
      printf("UNDEFINED\n");
      break;
    }
#endif
    printf("get_distance(left) : %d\n", sensor_.GetDistance(SensorID::LeftFront));
    printf("get_distance(right) : %d\n", sensor_.GetDistance(SensorID::RightFront));
    printf("End loop\n");
  }
  cv_task_flag_ = false;
  cv_thread_.join();
  cv::destroyAllWindows();
}
