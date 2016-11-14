#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "interface.h"

static void exit_program(int);

int
main()
{
  struct mstat mst;
  signal(SIGINT, exit_program);
  motor_init();  // initialize motor
  mstat_init(&mst); // initialize motorstat
  short left, right;
  while (1) {
    printf("Input [-1.0, 1.0]: ");
    scanf("%hd %hd", &left, &right);
	motor_set(&mst, left, right);
	motor_write(&mst);
  }
  return 0;
}

static void exit_program(int sig)
{
  //motor_exit_loop();
  motor_set_by_double(0.0);
  fprintf(stderr, "kill signal is received\n");
  motor_finalize();
  fprintf(stderr, "exiting...\n");
  exit(0);
}

