#ifndef ROBOT_CORE_CVISION_H_
#define ROBOT_CORE_CVISION_H_

class CVision
{
public:
  static CVision& getInstance(void) {
    static CVision singleton;
    return singleton;
  }
  //void Init();
  //void Finalize();
private:
	CVision();
};

#endif  // ROBOT_CORE_CVISION_H_
	