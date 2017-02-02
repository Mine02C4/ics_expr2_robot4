#ifndef ROBOT_CORE_GUN_H_
#define ROBOT_CORE_GUN_H_

#include <mutex>

class Gun
{
public:
  static Gun& getInstance(void) {
    static Gun singleton;
    return singleton;
  }
  void Init();
  void FireNum(int);
  void FireBurst(int);
  void FireSingle();
  void Finalize();
  void TurretAbsoluteElevate(int degrees);
  void TurretRelativeUp(int degrees);
  void TurretRelativeTurn(int degrees);
  int GetCurrentAngle();
private:
  Gun();
  void TurnAbsoluteDegrees(int degrees);
  int current_elevation_;
  const int kElevationUpperLimit = 70;
  const int kElevationLowerLimit = -20;
  int current_angle_;
  const int kAngleLimit = 90;
  std::mutex turn_mtx_;
  std::mutex elevate_mtx_;
};

#endif  // ROBOT_CORE_GUN_H_
