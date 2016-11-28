#ifndef STUB_LIBRARY_VIRTUALOBJECT_H_
#define STUB_LIBRARY_VIRTUALOBJECT_H_

#include <vector>

#include <opencv2/core/core.hpp>

namespace stub {

using Line = std::pair<cv::Vec2i, cv::Vec2i>;

class VirtualObject
{
public:
  VirtualObject();
  ~VirtualObject();
  virtual void Render(cv::Mat scene) = 0;
private:
};

class Rectangle : VirtualObject {
public:
  Rectangle(int width, int height);
  Rectangle(int x, int y, int width, int height);
  void Render(cv::Mat scene);
  std::vector<Line> GetEdges();
private:
  cv::Rect rect_;
};

};  // namespace stub

#endif  // STUB_LIBRARY_VIRTUALOBJECT_H_
