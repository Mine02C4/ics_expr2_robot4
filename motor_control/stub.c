#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../stub_library/common.h"

static int init_flag = 0;
static int finalize_flag = 0;

static void check_finalize(void);

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
  assert(init_flag == 1);
  printf("Start run forward\n");
  sleep_seconds(seconds);
  printf("Stop run forward\n");
}

void
turn_right(double seconds)
{
  assert(init_flag == 1);
  printf("Start turn right\n");
  TurnRightPeriodInSeconds(1.0);
  printf("Stop turn right\n");
}

void
turn_left(double seconds)
{
  assert(init_flag == 1);
  printf("Start turn left\n");
  TurnLeftPeriodInSeconds(1.0);
  printf("Stop turn left\n");
}

void
motor_finalize()
{
  finalize_flag = 1;
}

static void
check_finalize(void)
{
  if (init_flag == 1 && finalize_flag == 0) {
    fprintf(stderr, "Error: Not finalized\n");
  }
}

