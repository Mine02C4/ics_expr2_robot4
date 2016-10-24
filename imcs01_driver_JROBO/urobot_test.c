#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <signal.h>
#include "driver/urbtc.h"
#include "driver/urobotc.h"

void exit_program();

int quit_flag = 1;
int fd;

int main(int argc, char const* argv[])
{
	struct uin ibuf;	/* end point 1 */
	struct uout obuf;	 
	struct ccmd cmd;	/* end point 6 */
	int i;

	char *dev = "/dev/urbtc0";
	return 0;
}
