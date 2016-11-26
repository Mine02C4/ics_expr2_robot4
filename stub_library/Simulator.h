#ifndef STUB_LIBRARY_SIMULATOR_H_
#define STUB_LIBRARY_SIMULATOR_H_

#include <list>
#include <memory>
#include <mutex>
#include <thread>

#include "VirtualObject.h"

namespace stub {

class Simulator
{
public:
  static Simulator& getInstance(void) {
    static Simulator singleton;
    return singleton;
  }
  double direction_ = 0.0; // in degree
  double x = 100.0, y = 100.0;
  std::mutex mtx_;
private:
  Rectangle playground_;
  std::list<std::shared_ptr<VirtualObject>> vobjects_;

  Simulator();
  void Render();
};

};  // namespace stub

#endif  // STUB_LIBRARY_SIMULATOR_H_
