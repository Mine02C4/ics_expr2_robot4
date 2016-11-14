#include "interface.h"

#include <stdio.h>
#include <assert.h>

static init_flag = 0;

void
motor_init()
{
  printf("motor_init()\n");
  assert(init_flag == 0);
  init_flag = 1;
}

