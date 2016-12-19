#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>

#define BAUDRATE B9600

const static char *arduino_dev = "/dev/ttyACM0";
static int fd;
struct termios oldtio, newtio;

void turret_init()
{
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
}

void
open_fire(int n_bullets)
{
  char buf[255];
  sprintf(buf, "fire=%d\n", n_bullets);
  int size = strlen(buf);
  if ((write(fd, buf, size)) != size) {
    fprintf(stderr, "Error: Error open fire\n");
    exit(1);
  }
}

void
turret_finalize()
{
  close(fd);
}

