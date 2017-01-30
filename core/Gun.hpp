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
  void TurretUp();
  void TurretDown();
private:
  Gun();
  int current_elevation_;
};

#endif  // ROBOT_CORE_GUN_H_
