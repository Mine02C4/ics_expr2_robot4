#include "VirtualObject.h"

#include <opencv2/imgproc/imgproc.hpp>

namespace stub {

VirtualObject::VirtualObject()
{
}


VirtualObject::~VirtualObject()
{
}

Rectangle::Rectangle(int width, int height) :
  rect_(0, 0, width, height)
{
}

Rectangle::Rectangle(int x, int y, int width, int height) :
  rect_(x, y, width, height)
{
}

void Rectangle::Render(cv::Mat scene)
{
  cv::rectangle(scene, rect_, cv::Scalar(255, 0, 0));
}

std::vector<Line> Rectangle::GetEdges()
{
  return std::vector<Line> {
    Line(rect_.tl(), cv::Vec2d(rect_.x, rect_.y + rect_.height)),
      Line(cv::Vec2d(rect_.x, rect_.y + rect_.height), rect_.br()),
      Line(rect_.br(), cv::Vec2d(rect_.x + rect_.width, rect_.y)),
      Line(cv::Vec2d(rect_.x + rect_.width, rect_.y), rect_.tl())
  };
}

};  // namespace stub
