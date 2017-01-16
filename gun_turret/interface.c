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
#define DEPRESSION_LIM -20
#define ELEVATION_LIM 70

const static char *arduino_dev = "/dev/ttyACM0";
static int fd;
static int curr_ev, curr_ang;
struct termios oldtio, newtio;

void turret_init()
{
	curr_ev = 0; curr_ang = 0;	/* initialize current pos */
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
	open_fire(1);
	turn_by_degrees(0);
	sleep(2);
	elevate_by_degrees(0);
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
	// Absolute
	
	/*
	if (ANGLE_LIMIT <= curr_ang + degrees) {
		degrees = ANGLE_LIMIT - curr_ang;
	} else if (curr_ang + degrees <= -ANGLE_LIMIT) {	// neg
		degrees = -ANGLE_LIMIT - degrees;
	}
	*/
	if (-ANGLE_LIMIT <= degrees <= ANGLE_LIMIT) {
		curr_ang = degrees;
	} else {
		fprintf(stderr, "Invalid Angle\n");
		return;
	}

	char buf[BUFSIE];
 	int size = strlen(buf);
	sprintf(buf, "turn=%d\n", degrees);
  if ((write(fd, buf, size)) != size) {
    fprintf(stderr, "Error: Error turn by degrees\n");
    exit(1);
  }

	fprintf(stderr, "Aiming at; Deg: %2d Ev: %2d\n", curr_ang, curr_ev);
}

void
elevate_by_degrees(int degrees)
{
	// TODO: taking care of servo motor
	
	char buf[BUFSIE];
 	int size = strlen(buf);

	if (-DEPRESSION_LIM <= degrees <= ELEVATION_LIM) {
		curr_ev = degrees;
	} else {
		fprintf(stderr, "Invalid Angle\n");
		return;
	}

	sprintf(buf, "turret=%d\n", degrees);

  if ((write(fd, buf, size)) != size) {
    fprintf(stderr, "Error: Error elevate by degrees\n");
    exit(1);
  }
	fprintf(stderr, "Aiming at; Deg: %2d Ev: %2d\n", curr_ang, curr_ev);
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

