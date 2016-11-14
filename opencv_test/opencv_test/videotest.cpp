#include "videotest.h"

#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

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

  cv::resize(cv::imread("target.jpg"), target_img_, cv::Size(), 1, 1);
  cv::Ptr<cv::Feature2D> detector, extractor;
  detector = cv::AKAZE::create();;
  extractor = cv::AKAZE::create();;
  detector->detect(target_img_, target_keypts_);
  extractor->compute(target_img_, target_keypts_, target_desc_);
  for (int i = 0; i < target_keypts_.size(); ++i) {
    cv::circle(target_img_, target_keypts_[i].pt, 2, cv::Scalar(0, 200, 0), 1, CV_AA);
  }
  cv::imshow("Origin", target_img_);
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
  detector = cv::AKAZE::create();;
  extractor = cv::AKAZE::create();;
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
  matcher.knnMatch(target_desc_, desc, knn_matches, 3);

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
      match_point1.push_back(target_keypts_[knn_matches[i][0].queryIdx].pt);
      match_point2.push_back(kpts[knn_matches[i][0].trainIdx].pt);
    }
  }

  prev_keypts_ = kpts;
  prev_desc_ = desc;

  //ホモグラフィ行列推定
  cv::Mat masks;
  cv::Mat H;
  if (match_point1.size() != 0 && match_point2.size() != 0) {
    H = cv::findHomography(match_point1, match_point2, masks, cv::RANSAC, 3.f);
  }

  disp_features_ = frame_;
  for (int i = 0; i < kpts.size(); ++i) {
    cv::circle(disp_features_, kpts[i].pt, 2, cv::Scalar(200, 0, 0), 1, CV_AA);
  }
  for (int i = 0; i < match_point1.size(); i++) {
    //cv::line(disp_features_, match_point1[i], match_point2[i], cv::Scalar(0, 200, 0), 2, CV_AA);
    cv::circle(disp_features_, match_point2[i], 4, cv::Scalar(0, 200, 0), 1, CV_AA);
  }
  std::cout << match_point1.size() << std::endl;

  if (!H.empty()) {
    std::vector<cv::Point2f> obj_corners(4);
    obj_corners[0] = cv::Point2f(.0f, .0f);
    obj_corners[1] = cv::Point2f(static_cast<float>(target_img_.cols), .0f);
    obj_corners[2] = cv::Point2f(static_cast<float>(target_img_.cols), static_cast<float>(target_img_.rows));
    obj_corners[3] = cv::Point2f(.0f, static_cast<float>(target_img_.rows));
    std::vector<cv::Point2f> scene_corners(4);
    cv::perspectiveTransform(obj_corners, scene_corners, H);

    // コーナー間を線で結ぶ ( シーン中のマップされた対象物体 - シーン画像 )
    float w = static_cast<float>(target_img_.cols);
    cv::line(disp_features_, scene_corners[0] + cv::Point2f(w, .0f), scene_corners[1] + cv::Point2f(w, .0f), cv::Scalar(0, 255, 0), 4);
    cv::line(disp_features_, scene_corners[1] + cv::Point2f(w, .0f), scene_corners[2] + cv::Point2f(w, .0f), cv::Scalar(0, 255, 0), 4);
    cv::line(disp_features_, scene_corners[2] + cv::Point2f(w, .0f), scene_corners[3] + cv::Point2f(w, .0f), cv::Scalar(0, 255, 0), 4);
    cv::line(disp_features_, scene_corners[3] + cv::Point2f(w, .0f), scene_corners[0] + cv::Point2f(w, .0f), cv::Scalar(0, 255, 0), 4);

  }

  cv::imshow("Output", disp_features_);
}


