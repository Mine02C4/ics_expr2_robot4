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
static int curr_ang;
struct termios oldtio, newtio;

void sflush()
{
  tcflush(fd, TCIFLUSH);
  tcflush(fd, TCOFLUSH);
  return;
}

void turret_init()
{
  curr_ang = 0;
  fd = open(arduino_dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
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
  newtio.c_iflag=0;
  newtio.c_oflag=0;
  newtio.c_cflag=CS8|CREAD|CLOCAL;
  newtio.c_lflag=0;
  newtio.c_cc[VMIN]=1;
  newtio.c_cc[VTIME]=5;
  if (ioctl(fd, TCSETS, &newtio) < 0) {
    fprintf(stderr, "Error: cannot set termios.\n");
    exit(1);
  }
  fprintf(stderr, "Testing stepping motor\n");
  turn_by_degrees(0);
  sleep(2);
  elevate_by_degrees(0);
}

void
open_fire(int n_bullets)
{
  char buf[BUFSIE];
  sprintf(buf, "fire=%dx", n_bullets);
  int size = strlen(buf);
  if ((write(fd, buf, size)) != size) {
    fprintf(stderr, "Error: Error open fire\n");
    exit(1);
  }
  sflush();
}

void
burst_fire(int n_bullets)
{
  char buf[BUFSIE];
  sprintf(buf, "burst=%dx", n_bullets);
  int size = strlen(buf);
  if ((write(fd, buf, size)) != size) {
    fprintf(stderr, "Error: Error burst fire\n");
    exit(1);
  }
  sflush();
}

void
turn_by_degrees(int degrees)
{
	/* check valid input */
	// TODO: implement pid
	// Left: positive, Right: negative
	// Absolute
	
	if (-ANGLE_LIMIT <= curr_ang + degrees && curr_ang + degrees <= ANGLE_LIMIT) {
		curr_ang = degrees;
	} else {
		fprintf(stderr, "Invalid Angle\n");
		return;
	}

	char buf[BUFSIE];
	sprintf(buf, "turn=%dx", degrees);
 	int size = strlen(buf);

  if ((write(fd, buf, size)) != size) {
    fprintf(stderr, "Error: Error turn by degrees\n");
    exit(1);
  }
  sflush();

  fprintf(stderr, "Aiming at; Deg: %2d\n", curr_ang);
}

void
elevate_by_degrees(int degrees)
{
  char buf[BUFSIE];
  sprintf(buf, "turret=%dx", degrees);
  int size = strlen(buf);

  if ((write(fd, buf, size)) != size) {
    fprintf(stderr, "Error: Error elevate by degrees\n");
    exit(1);
  }
  sflush();
  fprintf(stderr, "Aiming at; Deg: %2d\n", curr_ang);
  return;
}

void
turret_finalize()
{
  /* revert to inital pos */
  turn_by_degrees(-curr_ang);
  elevate_by_degrees(0);
  sflush();
  close(fd);
  return;
}

