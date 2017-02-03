#include "CVision.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

CVision::CVision() {}

int CVision::Init(bool nowindow)
{
  show_window_ = !nowindow;
  cap_.open(0);
  if (!cap_.isOpened())
    return 1;
  cap_.set(CV_CAP_PROP_FRAME_WIDTH, camera_width_);
  cap_.set(CV_CAP_PROP_FRAME_HEIGHT, camera_height_);
  cap_.read(frame_);
  frame_updated_ = false;
  return 0;
}

void CVision::FetchFrame()
{
  if (cap_.grab() == false)
    return;
  std::lock_guard<std::mutex> lock(frame_mtx_);
  if (cap_.retrieve(last_frame_, 0) == true) {
    frame_updated_ = true;
  }
}

bool CVision::ReadFrame()
{
  std::lock_guard<std::mutex> lock(frame_mtx_);
  if (frame_updated_) {
    frame_ = last_frame_.clone();
    frame_updated_ = false;
    return true;
  }
  else {
    return false;
  }
}

bool  CVision::DetectPointer(Mat &rgb, Mat &hsv, Point &p, std::string &opt)
{
  Size size = rgb.size();
  cv::Mat mask(size, rgb.type());
  Mat hsv0[3];
  hsv0[0].create(hsv.size(), CV_8UC1);
  hsv0[1].create(hsv.size(), CV_8UC1);
  hsv0[2].create(hsv.size(), CV_8UC1);
  split(hsv, hsv0);
  Mat	m0(hsv.size(), CV_8UC1);
  Mat	m1(hsv.size(), CV_8UC1);
  threshold(hsv0[2], m0, 240, 255, CV_THRESH_BINARY);   // Vmin
  threshold(hsv0[1], m1, 50, 255, CV_THRESH_BINARY_INV);  // Smax
  cv::bitwise_and(m0, m1, mask);
  cv::Mat labels;
  cv::Mat stats;
  cv::Mat centroids;
  int n_label = cv::connectedComponentsWithStats(mask, labels, stats, centroids);
  int center_x = size.width / 2 + pointer_offset_.x;
  int center_y = size.height / 2 + pointer_offset_.y;
  for (int i = 1; i < n_label; ++i) {
    int *param = stats.ptr<int>(i);
    double *centroid = centroids.ptr<double>(i);
    double x = static_cast<double>(centroid[0]);
    double y = static_cast<double>(centroid[1]);
    int area = param[cv::ConnectedComponentsTypes::CC_STAT_AREA];
    if (
      x > center_x - pointer_detection_size_ / 2 &&
      x < center_x + pointer_detection_size_ / 2 &&
      y > center_y - pointer_detection_size_ / 2 &&
      y < center_y + pointer_detection_size_ / 2 &&
      area < 40
      ) {
      p.x = x;
      p.y = y;
      auto val = hsv.at<Vec3b>(y, x);
      opt = "area: " + std::to_string(area) +
        " H " + std::to_string(val[0]) +
        " S " + std::to_string(val[1]) +
        " V " + std::to_string(val[2]);
      return true;
    }
  }
  return false;
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
  cv::threshold(hsv0[0], m1, 110, 255, CV_THRESH_BINARY_INV);  // Hmax
  cv::threshold(hsv0[1], m2, 120, 255, CV_THRESH_BINARY);  // Smin
  cv::threshold(hsv0[2], m3, 40, 255, CV_THRESH_BINARY);  // Vmin
  cv::bitwise_and(m0, m1, mask);
  cv::bitwise_and(mask, m2, mask);
  cv::bitwise_and(mask, m3, mask);
}


bool CVision::DetectBlueBox(int & area, int & cx, int & cy)
{
  Size size = frame_.size();
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
  int center_x = size.width / 2 + pointer_offset_.x;
  int center_y = size.height / 2 + pointer_offset_.y;
  rectangle(
    output,
    cv::Rect(
      center_x - pointer_detection_size_ / 2,
      center_y - pointer_detection_size_ / 2,
      pointer_detection_size_,
      pointer_detection_size_
    ),
    cv::Scalar(0, 0, 255), 1
  );
  Point pointer;
  std::string opt;
  bool pointer_detected = DetectPointer(frame_, hsv, pointer, opt);
  if (pointer_detected) {
    circle(output, pointer, 5, cv::Scalar(0, 0, 255), 2);
  }

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
    area = largest_area;
    double *param = centroids.ptr<double>(largest_id);
    double x = static_cast<double>(param[0]);
    double y = static_cast<double>(param[1]);
    cx = 2048 * (x - pointer_offset_.x / size.width) - 1024;
    cy = 2048 * (y - pointer_offset_.y / size.height) - 1024;
    if (show_window_) { // DEBUG BLOCK
      int *param = stats.ptr<int>(largest_id);
      int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
      int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
      int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
      int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
      rectangle(output, cv::Rect(x, y, width, height), cv::Scalar(0, 255, 0), 2);
      std::stringstream num;
      num << largest_area << " cx:" << cx << " cy:" << cy;
      putText(output, num.str(), Point(x + 5, y + 20), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 255, 255), 1);

      cv::circle(output, cv::Point(x, y), 3, cv::Scalar(0, 255, 0), -1);
      cv::resize(output, output, Size(), 2.0, 2.0);
      if (pointer_detected) {
        std::stringstream pstr;
        pstr << "x:" << pointer.x << " y: " << pointer.y << "\n" << opt;
        putText(output, pstr.str(), Point(pointer.x * 2 + 5, pointer.x * 2 + 20), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 255), 1);
      }
      cv::imshow("Output", output);
    }
    return true;
  }
  else {
    return false;
  }

}
