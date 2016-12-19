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

void VideoTest::DetectionByColor()
{
  cv::Mat hsv(frame_.size(), frame_.type());
  cv::Mat mask(frame_.size(), frame_.type());
  cv::cvtColor(frame_, hsv, CV_BGR2HSV);
  cv::Mat bgr0[3], output;
  Recognition::DetectTargetBlue(hsv, mask);

  // adjust bitmask
  cv::blur(mask, mask, cv::Size(5, 5));
  cv::threshold(mask, mask, 204 - 1, 255, CV_THRESH_BINARY);

  cv::Mat labels;
  cv::Mat stats;
  cv::Mat centroids;
  int n_label = cv::connectedComponentsWithStats(mask, labels, stats, centroids);

  // remake original image
  cv::split(frame_, bgr0);

  cv::bitwise_and(bgr0[0], mask, bgr0[0]);
  cv::bitwise_and(bgr0[1], mask, bgr0[1]);
  cv::bitwise_and(bgr0[2], mask, bgr0[2]);

  cv::merge(bgr0, 3, output);
  
  int min_area = 2000;
  int largest_area = 0;
  int largest_id = 0;
  for (int i = 1; i < n_label; ++i) {
    int *param = stats.ptr<int>(i);
    int area = param[cv::ConnectedComponentsTypes::CC_STAT_AREA];
    if (largest_area < area) {
      largest_area = area;
      largest_id = i;
    }
  }

  if (largest_area > min_area) {
    int *param = stats.ptr<int>(largest_id);
    int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
    int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
    int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
    int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
    cv::rectangle(output, cv::Rect(x, y, width, height), cv::Scalar(0, 255, 0), 2);
    std::stringstream num;
    num << largest_area;
    cv::putText(output, num.str(), cv::Point(x + 5, y + 20), cv::FONT_HERSHEY_COMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);
  }

  for (int i = 1; i < n_label; ++i) {
    double *param = centroids.ptr<double>(i);
    int x = static_cast<int>(param[0]);
    int y = static_cast<int>(param[1]);
    cv::circle(output, cv::Point(x, y), 3, cv::Scalar(0, 0, 255), -1);
  }

  cv::imshow("Output", output);
}
