#ifndef STUB_LIBRARY_COMMON_H_
#define STUB_LIBRARY_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__
#include <unistd.h>
  static void sleep_seconds(double seconds) {
    usleep(seconds * 1000000);
  }
#elif _MSC_VER
#include <Windows.h>

  static void sleep_seconds(double seconds) {
    Sleep(seconds * 1000);
  }

#endif

#ifdef __cplusplus
}
#endif

#endif  // STUB_LIBRARY_COMMON_H_
