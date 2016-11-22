#include <stdio.h>

#include "interface.h"

int
main()
{
  sensor_init();
  int dist;
  dist = get_distance(S1);
  printf("Distance S1 = %d\n", dist);
  dist = get_distance(S2);
  printf("Distance S2 = %d\n", dist);
  sensor_finalize();
  return 0;
}

