#include <cstdio>

#include "../motor_control/interface.h"
#include "../i2c_sensor/interface.h"

int main()
{
  printf("Start!\n");
  motor_init();
  sensor_init();
  int dist = get_distance(S1);
  printf("Distance S1 d = %d\n", dist);
  run_forward(1.5);
  motor_finalize();
  sensor_finalize();
  return 0;
}

