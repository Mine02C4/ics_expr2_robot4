#ifndef ROBOT_CORE_DRIVE_H_
#define ROBOT_CORE_DRIVE_H_

class Drive
{
public:
  static Drive& getInstance(void) {
    static Drive singleton;
    return singleton;
  }
  void Init();
  void RunForward(double seconds);
  void Finalize();
private:
  Drive();
};

#endif  // ROBOT_CORE_DRIVE_H_
