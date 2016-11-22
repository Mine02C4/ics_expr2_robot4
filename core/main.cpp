#include <cstdio>

#include "../motor_control/interface.h"

int main()
{
  printf("Start!\n");
  motor_init();
  run_forward(1.5);
  motor_finalize();
  return 0;
}

