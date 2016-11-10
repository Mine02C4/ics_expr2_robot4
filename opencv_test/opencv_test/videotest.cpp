#include "videotest.h"

#include <iostream>

#include <opencv2/features2d.hpp>

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
  cv::namedWindow("Output", CV_WINDOW_AUTOSIZE);
  cap_.read(frame_);
  //disp_img_.create(cv::Size(320, 240), CV_8UC3);
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

void VideoTest::TrackFeatures()
{
  cv::Ptr<cv::Feature2D> detector, extractor;
  detector = cv::FeatureDetector::create("ORB");
  extractor = cv::FeatureDetector::create("ORB");
  std::vector<cv::KeyPoint> kpts;
  cv::Mat desc;
  detector->detect(frame_, kpts);
  extractor->compute(frame_, kpts, desc);
  if (desc.rows == 0) {
    std::cout << "No feature." << std::endl;
    return;
  }
  disp_features_ = frame_;
  for (int i = 0; i < kpts.size(); ++i) {
    cv::circle(disp_features_, kpts[i].pt, 5, cv::Scalar(200, 0, 0), 1, CV_AA);
  }
  cv::imshow("Output", disp_features_);
}


