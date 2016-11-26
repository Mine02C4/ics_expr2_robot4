#ifndef STUB_LIBRARY_VIRTUALOBJECT_H_
#define STUB_LIBRARY_VIRTUALOBJECT_H_

#include <opencv2/core/core.hpp>

namespace stub {

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
private:
  cv::Rect rect_;
};

};  // namespace stub

#endif  // STUB_LIBRARY_VIRTUALOBJECT_H_
