#ifndef ROBOT_CORE_GUN_H_
#define ROBOT_CORE_GUN_H_

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
private:
  Gun();
  int current_elevation_;
  const int kElevationUpperLimit = 70;
  const int kElevationLowerLimit = -20;
};

#endif  // ROBOT_CORE_GUN_H_
