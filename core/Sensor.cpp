#include "Sensor.hpp"

#include <map>

#include "../i2c_sensor/interface.h"

static sensor MapSensorID(SensorID sid)
{
  static std::map<SensorID, sensor> mp = {
    { SensorID::RightFront, S1 },
    { SensorID::LeftFront, S2 }
  };
  return mp[sid];
}

void Sensor::Init()
{
  sensor_init();
}

int Sensor::GetDistance(SensorID sid)
{
  return get_distance(MapSensorID(sid));
}

Sensor::Sensor()
{
}

void Sensor::Finalize()
{
  sensor_finalize();
}
