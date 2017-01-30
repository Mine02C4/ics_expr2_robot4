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
    
    gun_.TurnByDegrees(cx / 1024 * 60);
     
      if (area < 3000) {
        drive_.RunForward(1);
      }
      if (area > 5000) {
        drive_.RunForward(-1);
      }
      else {
        //gun adjustment
        if (cy < -512) {
          int degrees = 1 - cy / 10;
          gun_.TurretRelativeUp(1 - cy / 10);
          printf("MainLogic TurretRelativeUp %d\n", degrees);
        }
        else if (cy > 512) {
          int degrees = -1 - cy / 10;
          gun_.TurretRelativeUp(-1 - cy / 10);
          printf("MainLogic TurretRelativeUp %d\n", degrees);
        }
      }
  }
}

void MainLogic::StartPursuingBox()
{
  cv_thread_ = std::thread([this] {
    printf("Core MainLogic: Start StartPursuingBox\n");
    while (cv_task_flag_) {
      if (vision_.getInstance().ReadFrame()) {
        AdjustGunTurret();
      }
      int key = cv::waitKey(1) & 0xff;
      if (key == 27) {
        break;
      }
    }
    printf("Core MainLogic: End StartPursuingBox\n");
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
      drive_.RunForward(-500);
    }
    printf("get_distance(left) : %d\n", sensor_.GetDistance(SensorID::LeftFront));
    printf("get_distance(right) : %d\n", sensor_.GetDistance(SensorID::RightFront));
#ifndef _MSC_VER
    Wait_Voice_By_Code();
    //  std::string str = voice_.Wait_One_Sentence(5);
    /*int code = voice_.Wait_One_Code(5);
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
      }*/
#endif
    printf("get_distance(left) : %d\n", sensor_.GetDistance(SensorID::LeftFront));
    printf("get_distance(right) : %d\n", sensor_.GetDistance(SensorID::RightFront));
    printf("End loop\n");
  }
  cv_task_flag_ = false;
  cv_thread_.join();
  cv::destroyAllWindows();
}

#ifndef _MSC_VER
void MainLogic::Wait_Voice_By_Code() {
	static int mode = 0;
  struct voicecode vc;
  voice_.Wait_One_Code(5, vc);
  fprintf(stderr, "Voicerec:last sentence: %s\n", voice_.getString().c_str());
  fprintf(stderr, "Voicerec:code:%d num:%d\n", vc.code, vc.num);
  int num, dist;
  switch (vc.code) {
    case VC_CODE_MODECHANGE:
			mode++;
      printf("!Modechange\n");
			speech_.Speak("モード変更します");
			if (mode%2 == 0) {
				speech_.Speak("モード。シングルファイヤ");
			} else if (mode%2 == 1) {
				speech_.Speak("モード。バースト");
			}
      break;
    case VC_CODE_UCHIKATA:
      speech_.Speak("目標を殲滅します", ANGRY_FEEL);
			if (mode%2 == 0) {
				gun_.FireNum(1);
			} else if (mode%2 == 1) {
				gun_.FireBurst(1);
			}
      printf("!Uchikata\n");
      break;
    case VC_CODE_HOUTOU:
      printf("!Houtou\n");
    break;
    case VC_CODE_FIRE:
      printf("!FIRE\n");
      num  = vc.num;
      if (num >= 6) gun_.FireBurst(num/3);
      else gun_.FireNum(num);
      break;
    case VC_CODE_FORWARD:
      dist = vc.num;
      printf("RunForward\n");
      drive_.RunForward(dist * 10);
      break;
    case VC_CODE_BACK:
      dist = vc.num;
      printf("RunBack\n");
      drive_.RunForward(dist * -10);
      break;
    case VC_CODE_LEFT:
      dist = vc.num;
      printf("!Left\n");
      break;
    case VC_CODE_RIGHT:
      dist = vc.num;
      printf("!Right\n");
      break;
    case VC_CODE_ROTATE:
      dist = vc.num;
      printf("!Rotate\n");
      break;
    case VC_CODE_STOP:
      dist = vc.num;
      printf("!STOP\n");
      break;
    default:
      printf("UNDEFINED\n");
      break; 
  }
  return;
}
#endif
