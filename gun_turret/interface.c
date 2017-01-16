#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>

#define BAUDRATE B9600
#define BUFSIE 255
#define ANGLE_LIMIT 85

const static char *arduino_dev = "/dev/ttyACM0";
static int fd;
static int curr_ev, curr_ang;
struct termios oldtio, newtio;

void turret_init()
{
	curr_ev = 0; curr_ang = 0;
  fd = open(arduino_dev, O_RDWR);
  if (fd < 0) {
    fprintf(stderr, "Error: cannot open Arduino tty.\n");
    exit(1);
  }
  if (ioctl(fd, TCGETS, &oldtio) < 0) {
    fprintf(stderr, "Error: cannot get termios.\n");
    exit(1);
  }
  newtio = oldtio;
  cfsetispeed(&newtio, BAUDRATE);
  cfsetospeed(&newtio, BAUDRATE);
  if (ioctl(fd, TCSETS, &newtio) < 0) {
    fprintf(stderr, "Error: cannot set termios.\n");
    exit(1);
  }
	fprintf(stderr, "Testing stepping motor\n");
	open_fire(10);
//	turn_by_degrees(30);
	sleep(1);
//	turn_by_degrees(-60);
	sleep(2);
//	turn_by_degrees(30);
}

void
open_fire(int n_bullets)
{
  char buf[BUFSIE];
  sprintf(buf, "fire=%d\n", n_bullets);
  int size = strlen(buf);
  if ((write(fd, buf, size)) != size) {
    fprintf(stderr, "Error: Error open fire\n");
    exit(1);
  }
}

void
turn_by_degrees(int degrees)
{
	/* check valid input */
	// TODO: implement pid
	// Left: positive, Right: negative
	
	if (ANGLE_LIMIT <= curr_ang + degrees) {
		degrees = ANGLE_LIMIT - curr_ang;
	} else if (curr_ang + degrees <= -ANGLE_LIMIT) {	// neg
		degrees = -ANGLE_LIMIT - degrees;
	}
	
	char buf[BUFSIE];
  	int size = strlen(buf);
	if (degrees > 0) {
		sprintf(buf, "tl=%d\n", degrees);
	} else if (degrees < 0) {
		sprintf(buf, "tr=%d\n", degrees);
	} else {
		return;
	}
  if ((write(fd, buf, size)) != size) {
    fprintf(stderr, "Error: Error turn by degrees\n");
    exit(1);
  }
}

void
elevate_by_degrees(int degrees)
{
	// TODO: taking care of servo motor
	return;
}

void
turret_finalize()
{
	/* revert to inital pos */
	turn_by_degrees(-curr_ang);
	elevate_by_degrees(-curr_ev);
  close(fd);
	return;
}

