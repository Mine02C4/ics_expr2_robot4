#include "MainLogic.hpp"

#include <cstdio>

#include <string>

#include "CVision.hpp"

#include "../voice_recognition/voicecode.hpp"

#define MARGIN_EV_DEG 1
#define MARGIN_ROT_DEG 1
#define KY 0.01464
#define KX 0.01953

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
    if (area < 3000) {
      drive_.RunForward(5);
    }
    if (area > 5000) {
      drive_.RunForward(-5);
    }
    else {
      //gun adjustment
      if (cx * KX < -MARGIN_ROT_DEG || cx * KX > MARGIN_ROT_DEG) {
        int angle = static_cast<double>(cx * KX) ;
        gun_.TurretRelativeTurn(-angle);
        printf("MainLogic TurnByDegrees %d\n", angle);
      }
      else if (cy * KY < -MARGIN_EV_DEG) { // object is on upper
        int degrees = static_cast<double>(-cy * KY);
        gun_.TurretRelativeUp(degrees);
        printf("MainLogic TurretRelativeUp %d\n", degrees);
      }
      else if (cy * KY > MARGIN_EV_DEG) {
        int degrees = static_cast<double>(-(cy * KY));
        gun_.TurretRelativeUp(degrees);
        printf("MainLogic TurretRelativeUp %d\n", degrees);
      }
      else if (cy < 0) {
        // gun_.TurretRelativeUp(degrees);
      }
      else if (cy > 0) {
        //  gun_.TurretRelativeUp(degrees);
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

void MainLogic::StartCameraLoop()
{
  camera_thread_ = std::thread([this] {
    while (cv_task_flag_) {
      vision_.FetchFrame();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  });
}

void MainLogic::Launch()
{
  // Get command from other interfaces.
  cv_task_flag_ = true;
  StartCameraLoop();
  StartPursuingBox();
  for (;;) {
    int leftfront, rightfront;
    leftfront = sensor_.GetDistance(SensorID::LeftFront);
    rightfront = sensor_.GetDistance(SensorID::RightFront);
    if (leftfront < 20 || rightfront < 20) {
      printf("stop\n");
      drive_.RunForward(-50);
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
  camera_thread_.join();
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
  case VC_CODE_EXIT:
    speech_.Speak("終了します");
    fprintf(stderr, "MainLogic: exit by voice");
    // no finalize() function in class.
    exit(1);
  case VC_CODE_QUIZ:
    {
    speech_.Speak_Through("文章あてゲーム!パチパチパチ！", BASHFUL_FEEL);
    voice_.ChangeMode(FASTJCONF);
    speech_.Speak("君の喋った文章を当てるよ！");
    std::string sen = voice_.Wait_One_Sentence(10);
    speech_.Speak("もしかして君の喋った文章は");
    speech_.Speak(sen);
    speech_.Speak("ですか？");
    voice_.Wait_One_Code(5, vc);
    switch (vc.code) {
      case VC_CODE_CORRECT:
      speech_.Speak("やったー!");
      break;
      case VC_CODE_INCORRECT:
      speech_.Speak("え〜マジかー", SAD_FEEL);
      break;
      default:
        speech_.Speak("どうでしたか?");
      break;
    }
    speech_.Speak_Through("操作モードに戻ります");
    voice_.ChangeMode(MINEJCONF);
  }
  case VC_CODE_MODECHANGE:
    mode++;
    printf("!Modechange\n");
    speech_.Speak("モード変更");
    if (mode % 2 == 0) {
      speech_.Speak("モード。シングルファイヤ");
    }
    else if (mode % 2 == 1) {
      speech_.Speak("モード。バースト");
    }
    break;
  case VC_CODE_UCHIKATA:
    speech_.Speak("目標を殲滅", ANGRY_FEEL);
    if (mode % 2 == 0) {
      gun_.FireNum(1);
    }
    else if (mode % 2 == 1) {
      gun_.FireBurst(1);
    }
    printf("!Uchikata\n");
    break;
  case VC_CODE_HOUTOU:
    printf("!Houtou\n");
    if (vc.num == 1) { //up
//      gun_.TurretAbsoluteDegrees(30);
    }
    else if (vc.num == 2) { //down
//      gun_.TurretAbsoluteElevate(-20);
    }
    else if (vc.num == 3) {//right
//      gun_.TurretAbsoluteDegrees(45);
    }
    else if (vc.num == 4) { //left
//      gun_.TurretAbsoluteDegrees(-45);
    }
    break;
  case VC_CODE_FIRE:
    printf("!FIRE\n");
    num = vc.num;
    if (num >= 6) gun_.FireBurst(num / 3);
    else gun_.FireNum(num);
    break;
  case VC_CODE_FORWARD:
    dist = vc.num;
    printf("RunForward:%d\n", dist);
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
    drive_.TurnRight(vc.num);
    break;
  case VC_CODE_RIGHT:
    dist = vc.num;
    drive_.TurnRight(vc.num);
    printf("!Right\n");
    break;
  case VC_CODE_ROTATE:
    dist = vc.num;
    printf("!Rotate\n");
    drive_.Turn(vc.num);
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
