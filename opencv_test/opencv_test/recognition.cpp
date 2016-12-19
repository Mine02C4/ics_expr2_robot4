#include "recognition.h"

#include <opencv2/imgproc/imgproc.hpp>

void Recognition::DescribeFeatures(cv::Mat &image, FeatureDescription &fd)
{
  cv::Ptr<cv::Feature2D> detector, extractor;
  detector = cv::AKAZE::create();;
  extractor = cv::AKAZE::create();;
  detector->detect(image, std::get<0>(fd));
  extractor->compute(image, std::get<0>(fd), std::get<1>(fd));
}

void Recognition::MatchKeyPoints(
  FeatureDescription &fd1, FeatureDescription &fd2,
  std::vector<cv::DMatch> &good_matches,
  std::vector<cv::Point2f> &match_point1, std::vector<cv::Point2f> &match_point2)
{
  cv::BFMatcher matcher;
  std::vector<std::vector<cv::DMatch> > knn_matches;
  matcher.knnMatch(std::get<1>(fd1), std::get<1>(fd2), knn_matches, 3);
  const auto match_par = .6f;
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
      match_point1.push_back(std::get<0>(fd1)[knn_matches[i][0].queryIdx].pt);
      match_point2.push_back(std::get<0>(fd2)[knn_matches[i][0].trainIdx].pt);
    }
  }
}

void Recognition::DetectSkin(cv::Mat &hsv, cv::Mat &mask)
{
  cv::Mat hsv0[3];
  hsv0[0].create(hsv.size(), CV_8UC1);
  hsv0[1].create(hsv.size(), CV_8UC1);
  hsv0[2].create(hsv.size(), CV_8UC1);
  cv::split(hsv, hsv0);
  cv::Mat	m0(hsv.size(), CV_8UC1);
  cv::Mat	m1(hsv.size(), CV_8UC1);
  cv::Mat	m2(hsv.size(), CV_8UC1);
  cv::Mat	m3(hsv.size(), CV_8UC1);
  cv::threshold(hsv0[0], m0, 2 - 1, 255, CV_THRESH_BINARY);   // Hmin
  cv::threshold(hsv0[0], m1, 26, 255, CV_THRESH_BINARY_INV);  // Hmax
  cv::threshold(hsv0[1], m2, 49 - 1, 255, CV_THRESH_BINARY);  // Smin
  cv::threshold(hsv0[2], m3, 28 - 1, 255, CV_THRESH_BINARY);  // Vmin
  cv::bitwise_and(m0, m1, mask);
  cv::bitwise_and(mask, m2, mask);
  cv::bitwise_and(mask, m3, mask);
}

void Recognition::DetectTargetBlue(cv::Mat &hsv, cv::Mat &mask)
{
  cv::Mat hsv0[3];
  hsv0[0].create(hsv.size(), CV_8UC1);
  hsv0[1].create(hsv.size(), CV_8UC1);
  hsv0[2].create(hsv.size(), CV_8UC1);
  cv::split(hsv, hsv0);
  cv::Mat	m0(hsv.size(), CV_8UC1);
  cv::Mat	m1(hsv.size(), CV_8UC1);
  cv::Mat	m2(hsv.size(), CV_8UC1);
  cv::Mat	m3(hsv.size(), CV_8UC1);
  cv::threshold(hsv0[0], m0, 90, 255, CV_THRESH_BINARY);   // Hmin
  cv::threshold(hsv0[0], m1, 120, 255, CV_THRESH_BINARY_INV);  // Hmax
  cv::threshold(hsv0[1], m2, 49 - 1, 255, CV_THRESH_BINARY);  // Smin
  cv::threshold(hsv0[2], m3, 28 - 1, 255, CV_THRESH_BINARY);  // Vmin
  cv::bitwise_and(m0, m1, mask);
  cv::bitwise_and(mask, m2, mask);
  cv::bitwise_and(mask, m3, mask);
}
