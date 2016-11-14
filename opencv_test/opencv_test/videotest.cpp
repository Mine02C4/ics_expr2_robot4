#include "videotest.h"

#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>

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
  detector = cv::ORB::create();;
  extractor = cv::ORB::create();;
  std::vector<cv::KeyPoint> kpts;
  cv::Mat desc;

  // 特徴点検出と特徴量記述
  detector->detect(frame_, kpts);
  extractor->compute(frame_, kpts, desc);
  if (desc.rows == 0) {
    std::cout << "No feature." << std::endl;
    return;
  }

  // 特徴点マッチング
  cv::BFMatcher matcher;
  std::vector<std::vector<cv::DMatch> > knn_matches;
  matcher.knnMatch(prev_desc_, desc, knn_matches, 2);

  const auto match_par = .6f;
  std::vector<cv::DMatch> good_matches;
  std::vector<cv::Point2f> match_point1;
  std::vector<cv::Point2f> match_point2;
  bool flag = false;
  for (int i = 0; i < knn_matches.size(); ++i) {
    if (knn_matches[i].size() == 1) {
      flag = true;
    }
    else if (knn_matches[i].size() > 1) {
      auto dist1 = knn_matches[i][0].distance;
      auto dist2 = knn_matches[i][1].distance;
      flag = dist1 <= dist2 * match_par;
    }
    if (flag) {
      good_matches.push_back(knn_matches[i][0]);
      match_point1.push_back(prev_keypts_[knn_matches[i][0].queryIdx].pt);
      match_point2.push_back(kpts[knn_matches[i][0].trainIdx].pt);
    }
  }

  prev_keypts_ = kpts;
  prev_desc_ = desc;

  disp_features_ = frame_;
  for (int i = 0; i < kpts.size(); ++i) {
    cv::circle(disp_features_, kpts[i].pt, 2, cv::Scalar(200, 0, 0), 1, CV_AA);
  }
  for (int i = 0; i < match_point1.size(); i++) {
    cv::line(disp_features_, match_point1[i], match_point2[i], cv::Scalar(0, 200, 0), 2, CV_AA);
    cv::circle(disp_features_, match_point2[i], 4, cv::Scalar(0, 200, 0), 1, CV_AA);
  }
  cv::imshow("Output", disp_features_);
}


