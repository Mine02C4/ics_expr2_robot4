#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#define DEBUG

// set motor channel
#define MLEFT 1
#define MRIGHT 2

#include "./driver/urbtc.h"
#include "./driver/urobotc.h"

#include "interface.h"

const static char *devfile = "/dev/urbtc0";

static struct uin ibuf;
static struct uout obuf;
static struct ccmd cmd;
static int fd, fds;
static struct mstat mst;

static int motor_quit_flag = 1;

void mstat_init(struct mstat *mstp) {
	/* initialize motor status */
	mstp -> stat = STAT_DEFAULT;
	mstp -> rstat = STAT_DEFAULT;
	mstp -> lstat = STAT_DEFAULT;
	mstp -> motor_l = 0; 	/* set rotation */
	mstp -> motor_r = 0;
	return;
}

void
motor_init()
{
  int i;
  if ((fd = open(devfile, O_RDWR)) == -1) {
    fprintf(stderr, "%s: Open error\n", devfile);
    exit(1);
  }
  if (ioctl(fd, URBTC_CONTINUOUS_READ) < 0){
    fprintf(stderr, "ioctl: URBTC_CONTINUOUS_READ error\n");
    exit(1);
  }
  if (read(fd, &ibuf, sizeof(ibuf)) != sizeof(ibuf)) {
    fprintf(stderr, "Read size mismatch.\n");
    exit(1);
  }
  if (ibuf.magicno == 0) {
    fds = fd;
    fprintf(stderr, "Found controller #0.\n");
  } else {
    fprintf(stderr, "Wrong magic no: %d.\n", ibuf.magicno);
    exit(1);
  }

  cmd.retval = 0 /* RETURN_VAL */;
  cmd.setoffset  = CH0 | CH1 | CH2 | CH3;
  cmd.setcounter = CH0 | CH1 | CH2 | CH3;
  cmd.resetint   = CH0 | CH1 | CH2 | CH3;

  cmd.selin = CH2 | CH1 | SET_SELECT; /* AD in:ch0,ch1    ENC in:ch2,ch3*/
  cmd.selout = SET_SELECT | CH0 | CH1 | CH2 | CH3; /*  PWM out:ch0,ch1,ch2,ch3*/

  cmd.offset[0] = cmd.offset[1] = cmd.offset[2] = cmd.offset[3] = 0x7fff;
  cmd.counter[0] = cmd.counter[1] = cmd.counter[2] = cmd.counter[3] = 0;

  cmd.posneg = SET_POSNEG | CH0| CH1 | CH2 | CH3; /*POS PWM out*/
  cmd.breaks = SET_BREAKS | CH0 | CH1 | CH2 | CH3; /*No Brake*/

  if (ioctl(fd, URBTC_COUNTER_SET) < 0){
    fprintf(stderr, "ioctl: URBTC_COUNTER_SET error\n");
    exit(1);
  }
  if (write(fd, &cmd, sizeof(cmd)) < 0) {
    fprintf(stderr, "write error\n");
    exit(1);
  }
  if (ioctl(fd, URBTC_DESIRE_SET) < 0){
    fprintf(stderr, "ioctl: URBTC_DESIRE_SET error\n");
    exit(1);
  }

  for (i=0; i<4; i++) {
    obuf.ch[i].x = 1;	// x
    obuf.ch[i].d = 1; 	// v
    obuf.ch[i].kp = 1;
    obuf.ch[i].kpx = 1;
    obuf.ch[i].kd = 0;
    obuf.ch[i].kdx = 1;
    obuf.ch[i].ki = 0;
    obuf.ch[i].kix = 1;
  }
  /*
  obuf.ch[MLEFT].kp = 0x10;
  obuf.ch[MRIGHT].kp = 0x10;
  */
}


void set_stat (struct mstat *statp) {
  /* set status */

  statp -> stat &= ~(STAT_TL | STAT_TR | STAT_MVF);
  if (statp->motor_l < statp->motor_r) {
    statp->stat |= STAT_TL; // turning left
  } else if (statp->motor_l < statp->motor_r) {
    statp->stat |= STAT_TR; // turning right
  } else {
    statp->stat |= STAT_MVF; // fwd
  }
  return;
}

void print_obuf() {
	int i;
  for (i=0; i<4; i++) {
	  fprintf(stderr, "[ID: %d][x: %hd][kp: %hd][kpx: %hd][kd: %hd][kdx: %hd][ki: %hd][kix: %hd]\n",
			  i, obuf.ch[i].x, obuf.ch[i].kp, obuf.ch[i].kd, obuf.ch[i].kdx, obuf.ch[i].ki,
			  obuf.ch[i].kix);
  }
  return;
}

void motor_set(struct mstat *mstp, short rotl, short rotr){
	/* set both right and left rotation */
	if (((-1023 < rotl) && (rotl < 1023)) && 
			((-1023 < rotr) && (rotr < 1023))) {
		// if valid input
		mstp -> motor_l = rotl;
		mstp -> motor_r = rotr;
	} else {
		fprintf(stderr, "Invalid rot input. Rot should be between -1023 and 1023\n");
	}
	return;
}

int motor_write (struct mstat *statp) {
  /* todo one of the motor should be reversed */
  short rotl = statp -> motor_l; // left motor rotation
  short rotr = statp -> motor_r; // 512
  rotr = -rotr;
  unsigned short test = 512;
  obuf.ch[MRIGHT].kp = rotr << 5; // set right rounds
  obuf.ch[MLEFT].kp = rotl << 5; // set left rounds
  if (write(fd, &obuf, sizeof(obuf)) > 0) {
    printf("MRIGHT: %hd MLEFT: %hd\r\n", 
        obuf.ch[MRIGHT].x, obuf.ch[MLEFT].x);
  } else {
    printf("write err\n");
  }
  return 0;
}

int is_stat (unsigned short currstat, unsigned short statbit) {
  	/* return True or False */
	return ((currstat & statbit) == statbit);
}


// TODO: implement
void
run_forward(double seconds){
	motor_set(&mst, 100, 100);
	motor_write(&mst);
	usleep(seconds * 1000000);
	motor_set(&mst, 0, 0);
	motor_write(&mst);
	return;
}

// TODO: implement
void
turn_right(double seconds) {
	motor_set(&mst, 100, -100);
	motor_write(&mst);
	usleep(seconds * 1000000);
	motor_set(&mst, 0, 0);
	motor_write(&mst);
	return;
}

// TODO: implement
void
turn_left(double seconds) {
	motor_set(&mst, -100, 100);
	motor_write(&mst);
	usleep(seconds * 1000000);
	motor_set(&mst, 0, 0);
	motor_write(&mst);
	return;

}


void
motor_finalize()
{
  close(fds);
}

void
motor_exit_loop()
{
  motor_quit_flag = 0;
}

static int
ifn_by_wheel(enum wheel w) // interface number
{
  switch (w) {
    case LEFT:
      return MRIGHT;
    case RIGHT:
      return MLEFT;
    default:
      return -1; // Assertion error!
  }
}

