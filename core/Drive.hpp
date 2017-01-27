#ifndef ROBOT_CORE_DRIVE_H_
#define ROBOT_CORE_DRIVE_H_

#define CONV_DIST_TO_ROT 100

class Drive
{
public:
  static Drive& getInstance(void) {
    static Drive singleton;
    return singleton;
  }
  void Init();
	void RunForward(double rot);
	void TurnRight(double angle);
	void TurnLeft(double angle);
	void Turn(double angle);
  void SetDesire(double [2], double [2]);
  void RunForwardPeriodInSeconds(double seconds);
  void TurnRightPeriodInSeconds(double seconds);
  void TurnLeftPeriodInSeconds(double seconds);
	void SetCurrentPosition(int [2]);
  void Finalize();
private:
  Drive();
	double cval[3] = {0.0 , 0.0, 0.0};
};

#endif  // ROBOT_CORE_DRIVE_H_
