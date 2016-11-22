#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

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
  usleep(seconds * 1000000);
  printf("Stop run forward\n");
}

void
turn_right(double seconds)
{
  printf("Start turn right\n");
  usleep(seconds * 1000000);
  printf("Stop turn right\n");
}

void
turn_left(double seconds)
{
  printf("Start turn left\n");
  usleep(seconds * 1000000);
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

