#include "videotest.h"

#include <iostream>
#include <tuple>

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

  cv::resize(cv::imread("target.jpg"), target_img_, cv::Size(), 1, 1);
  Recognition::DescribeFeatures(target_img_, target_fd_);
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
  FeatureDescription fd;
  Recognition::DescribeFeatures(frame_, fd);
  if (std::get<1>(fd).rows == 0) {
    std::cout << "No feature." << std::endl;
    return;
  }

  std::vector<cv::Point2f> match_point1;
  std::vector<cv::Point2f> match_point2;
  std::vector<cv::DMatch> good_matches;
  Recognition::MatchKeyPoints(target_fd_, fd, good_matches, match_point1, match_point2);

  prev_fd_ = fd;

  //ホモグラフィ行列推定
  cv::Mat masks;
  cv::Mat H;
  if (match_point1.size() != 0 && match_point2.size() != 0) {
    H = cv::findHomography(match_point1, match_point2, masks, cv::RANSAC, 3.f);
  }

  disp_features_ = frame_;
  cv::drawMatches(target_img_, std::get<0>(target_fd_), frame_, std::get<0>(fd), good_matches, disp_features_,
    cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector< char >(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
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


