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
	motor_set_rot(&mst, left, right);
	motor_write(&mst);
	print_obuf();
  }
  return 0;
}

static void exit_program(int sig)
{
  fprintf(stderr, "kill signal is received\n");
  motor_finalize();
  fprintf(stderr, "exiting...\n");
  exit(0);
}

