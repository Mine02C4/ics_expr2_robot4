#ifndef ROBOT_CORE_SENSOR_H_
#define ROBOT_CORE_SENSOR_H_

enum class SensorID
{
  RightFront,
  LeftFront
};

class Sensor
{
public:
  static Sensor& getInstance(void) {
    static Sensor singleton;
    return singleton;
  }
  void Init();
  int GetDistance(SensorID sid);
  void Finalize();
private:
  Sensor();
};

#endif  // ROBOT_CORE_SENSOR_H_
