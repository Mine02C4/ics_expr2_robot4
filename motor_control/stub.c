#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../stub_library/common.h"

static int init_flag = 0;
static int finalize_flag = 0;

void check_finalize();

void
motor_init()
{
  atexit(check_finalize);
  printf("motor_init()\n");
  assert(init_flag == 0);
  init_flag = 1;
}

void
run_forward(double seconds)
{
  printf("Start run forward\n");
  sleep_seconds(seconds);
  printf("Stop run forward\n");
}

void
turn_right(double seconds)
{
  printf("Start turn right\n");
  sleep_seconds(seconds);
  printf("Stop turn right\n");
}

void
turn_left(double seconds)
{
  printf("Start turn left\n");
  sleep_seconds(seconds);
  printf("Stop turn left\n");
}

void
motor_finalize()
{
  finalize_flag = 1;
}

void
check_finalize()
{
  if (init_flag == 1 && finalize_flag == 0) {
    fprintf(stderr, "Error: Not finalized\n");
  }
}

