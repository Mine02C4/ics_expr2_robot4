#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../stub_library/common.h"

static int init_flag = 0;
static int finalize_flag = 0;

static void check_finalize(void);

void
turret_init()
{
  atexit(check_finalize);
  printf("turret_init()\n");
  assert(init_flag == 0);
  init_flag = 1;
}

void
open_fire(int n_bullets)
{
  assert(init_flag == 1);
  printf("Open fire\n.");
}

void
turn_by_degrees(int degrees) {
  assert(init_flag == 1);
  printf("Turn degree = %d\n.", degrees);
}

void
elevate_by_degrees(int degrees)
{
  assert(init_flag == 1);
  printf("Elevate degree = %d\n.", degrees);
}

void
turret_finalize()
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

