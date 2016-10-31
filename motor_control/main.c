#include <stdio.h>
#include <signal.h>

#include "interface.h"

static void exit_program(int);

int
main()
{
  signal(SIGINT, exit_program);
  motor_init();
  motor_test_loop();
  return 0;
}

static void exit_program(int sig)
{
  motor_exit();
  fprintf(stderr, "kill signal is received\n");
}

