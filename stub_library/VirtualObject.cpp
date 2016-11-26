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

};  // namespace stub
