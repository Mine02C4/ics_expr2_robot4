#ifndef STUB_LIBRARY_SIMULATOR_H_
#define STUB_LIBRARY_SIMULATOR_H_

#include <mutex>
#include <list>

#include "VirtualObject.h"

namespace stub {

class Simulator
{
public:
  static Simulator& getInstance(void) {
    static Simulator singleton;
    return singleton;
  }
private:
  std::mutex mtx;
  Rectangle &playground_;
  std::list<std::shared_ptr<VirtualObject>> vobjects_;

  Simulator();
  static Rectangle& InitPlayground();
  void Render();
};

};  // namespace stub

#endif  // STUB_LIBRARY_SIMULATOR_H_
