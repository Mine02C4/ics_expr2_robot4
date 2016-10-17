#include "videotest.h"

VideoTest::VideoTest()
{
}

VideoTest::~VideoTest()
{
  cap_.release();
  frame_.release();
  cv::destroyAllWindows();
}

int VideoTest::Init()
{
  cap_.open(0);
  if (!cap_.isOpened())
    return 1;
  cap_.set(CV_CAP_PROP_FRAME_WIDTH, 320);
  cap_.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
  cv::namedWindow("VideoTest", CV_WINDOW_AUTOSIZE);
  cap_.read(frame_);
  disp_img_.create(cv::Size(320, 240), CV_8UC3);
  return 0;
}

void VideoTest::ReadFrame()
{
  if (cap_.grab() == false)
    return;
  if (cap_.retrieve(frame_, 0) == false)
    return;
  cv::imshow("VideoTest", frame_);
}
