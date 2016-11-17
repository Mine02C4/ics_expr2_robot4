#include "recognition.h"

void Recognition::MatchKeyPoints(
  std::vector<cv::KeyPoint> &keypts1, cv::Mat &desc1,
  std::vector<cv::KeyPoint> &keypts2, cv::Mat &desc2,
  std::vector<cv::Point2f> &match_point1, std::vector<cv::Point2f> &match_point2)
{
  cv::BFMatcher matcher;
  std::vector<std::vector<cv::DMatch> > knn_matches;
  matcher.knnMatch(desc1, desc2, knn_matches, 3);
  const auto match_par = .6f;
  std::vector<cv::DMatch> good_matches;
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
      match_point1.push_back(keypts1[knn_matches[i][0].queryIdx].pt);
      match_point2.push_back(keypts2[knn_matches[i][0].trainIdx].pt);
    }
  }
}
