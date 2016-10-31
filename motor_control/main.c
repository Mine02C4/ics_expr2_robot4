#include <stdio.h>
#include <signal.h>

#include "interface.h"

static void exit_program(int);

int
main()
{
  signal(SIGINT, exit_program);
  motor_init();
  //motor_test_loop();
  while (1) {
    double val;
    printf("Input double [-1.0, 1.0]: ");
    scanf("%lf", &val);
    motor_set_by_double(val);
  }
  return 0;
}

static void exit_program(int sig)
{
  //motor_exit_loop();
  motor_set_by_double(0.0);
  fprintf(stderr, "kill signal is received\n");
  motor_finalize();
}

