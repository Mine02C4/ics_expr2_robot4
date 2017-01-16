#include "CVision.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

CVision::CVision() {}

int CVision::Init()
{
  cap_.open(0);
  if (!cap_.isOpened())
    return 1;
  cap_.set(CV_CAP_PROP_FRAME_WIDTH, 320);
  cap_.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
  cv::namedWindow("CVision", CV_WINDOW_AUTOSIZE);
  cv::namedWindow("Output", CV_WINDOW_AUTOSIZE);
  cap_.read(frame_);
  return 0;
}

void CVision::ReadFrame()
{
  if (cap_.grab() == false)
    return;
  if (cap_.retrieve(frame_, 0) == false)
    return;
  //cv::imshow("VideoTest", frame_);
}

void CVision::DetectTargetBlue(cv::Mat &hsv, cv::Mat &mask)
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


bool CVision::DetectBlueBox(int & area, int & cx, int & cy)
{

  cv::Mat hsv(frame_.size(), frame_.type());
  cv::Mat mask(frame_.size(), frame_.type());
  cv::cvtColor(frame_, hsv, CV_BGR2HSV);
  cv::Mat bgr0[3], output;
  DetectTargetBlue(hsv, mask);

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

  int min_area = 500; // S1
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
    { // DEBUG BLOCK
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
    area = largest_area;
    double *param = centroids.ptr<double>(largest_id);
    int x = static_cast<int>(param[0]);
    int y = static_cast<int>(param[1]);
    Size size = frame_.size();
    cx = 2048 * (x / size.width) - 1024;
    cy = 2048 * (y / size.height) - 1024;
    cv::circle(output, cv::Point(cx, cy), 3, cv::Scalar(0, 0, 255), -1);
    cv::imshow("Output", output);
    return true;
  }
  else {
    return false;
  }

}
