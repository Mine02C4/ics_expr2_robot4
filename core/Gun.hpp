#ifndef ROBOT_CORE_GUN_H_
#define ROBOT_CORE_GUN_H_

class Gun
{
public:
  static Gun& getInstance(void) {
    static Gun singleton;
    return singleton;
  }
  //void Init();
  //void Finalize();
private:
  Gun();
};

#endif  // ROBOT_CORE_GUN_H_
