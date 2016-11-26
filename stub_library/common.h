#ifndef STUB_LIBRARY_COMMON_H_
#define STUB_LIBRARY_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>

  static void sleep_seconds(double seconds) {
    usleep(seconds * 1000000);
  }

#elif _MSC_VER
#include <Windows.h>

  static void sleep_seconds(double seconds) {
    Sleep((DWORD)(seconds * 1000));
  }

#endif

#ifdef __cplusplus
}
#endif

#endif  // STUB_LIBRARY_COMMON_H_
