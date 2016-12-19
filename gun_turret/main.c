#include <stdio.h>

#include "interface.h"

int
main()
{
  turret_init();
  open_fire(1);
  turret_finalize();
  return 0;
}

