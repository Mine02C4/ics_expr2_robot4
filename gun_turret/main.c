#include <stdio.h>

#include "interface.h"

int
main()
{
  turret_init();
	int input;
	while (1) {
		fprintf(stderr, "MODE: (1:fire, 2:turn, 3: elevate)\n");
		scanf("%d", &input);
		switch (input) {
			case 1:
				fprintf(stderr, "FIRE: (number of bullets)\n");
				scanf("%d", &input);
				open_fire(input);
				break;
			case 2:
				fprintf(stderr, "TURN: (degrees)\n");
				scanf("%d", &input);
				turn_by_degrees(input);
				break;
			case 3:
				fprintf(stderr, "ELEVATE (degrees)\n");
				scanf("%d", &input);
				elevate_by_degrees(input);
				break;

			default:
				fprintf(stderr, "Invalid\n");
				break;
		}
	}
  turret_finalize();
  return 0;
}

