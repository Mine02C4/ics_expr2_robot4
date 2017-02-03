#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../stub_library/common.h"

static int init_flag = 0;
static int finalize_flag = 0;

static void check_finalize(void);

void
sensor_init()
{
  atexit(check_finalize);
  printf("sensor_init()\n");
  assert(init_flag == 0);
  init_flag = 1;
}

int
get_distance(enum sensor s)
{
  assert(init_flag == 1);
  int id;
  int range;
  switch (s) {
  case S1:
    id = 0;
    range = GetDistance(RIGHT_FRONT);
    break;
  case S2:
    id = 1;
    range = GetDistance(LEFT_FRONT);
    break;
  default:
    fprintf(stderr, "Error: Unknown distance sensor.\n");
    exit(1);
  }
  //printf("i2c_sensor: get_distance sensor = %d, dist = %d\n", id, range);
  return range;
}

void
sensor_finalize()
{
  finalize_flag = 1;
}

static void
check_finalize(void)
{
  if (init_flag == 1 && finalize_flag == 0) {
    fprintf(stderr, "Error: i2c_sensor not finalized\n");
  }
}

