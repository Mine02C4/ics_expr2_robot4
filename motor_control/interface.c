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
#define MLEFT 2
#define MRIGHT 3

#include "./driver/urbtc.h"
#include "./driver/urobotc.h"
#include "interface.h"

const static char *devfile = "/dev/urbtc0";

void report_error_and_exit(const char* msg, int rpid);

static struct uin ibuf;
static struct uout obuf;
static struct ccmd cmd;
static int fd, fds;
static struct mstat mst;

static int motor_quit_flag = 1;

void speed_up(int speed){
	speed += 5;
}

void mstat_init(struct mstat *mstp) {
	/* initialize motor status */
	mstp -> stat = STAT_DEFAULT;
	mstp -> rstat = STAT_DEFAULT;
	mstp -> lstat = STAT_DEFAULT;
	mstp -> rot_l = 0; 	/* set rotation */
	mstp -> rot_r = 0;
	mstp -> spd_l = 0;
	mstp -> spd_r = 0;
	return;
}

void print_fbk() {
	int i;
	if (ioctl(fd, URBTC_CONTINUOUS_READ) < 0) report_error_and_exit("printfbk_ioctl", 13);
    if ((i = read(fd, &ibuf, sizeof(ibuf))) != sizeof(ibuf)) {
      fprintf(stderr,
			  "Warning: read size mismatch (%d!=%d).\n",
			  i, sizeof(ibuf));
    }
    for (i=0; i<4; i++) {
      ibuf.ad[i] = ibuf.ad[i] >> 5;
    }

    printf("[time: %d][ad: %d %d]\n [ct: %d %d] [da: %d %d %d %d] [din: %x] [dout: %x] [imax: %d] [intv: %d]\r\n",
	   ibuf.time,  ibuf.ad[MLEFT], ibuf.ad[MRIGHT],
	   ibuf.ct[MLEFT], ibuf.ct[MRIGHT],
	   ibuf.da[MLEFT], ibuf.da[MRIGHT],
	   ibuf.din, ibuf.dout, ibuf.intmax, ibuf.interval);
}

void
motor_init()
{
  int i;
	fprintf(stderr, "Initializing motor...\n");
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


	cmd.selin =  MLEFT | MRIGHT | SET_SELECT;
  cmd.selout = SET_SELECT | CH0 | CH1 | CH2 | CH3; /*  PWM out:ch0,ch1,ch2,ch3*/

  cmd.offset[0] = cmd.offset[1] = cmd.offset[2] = cmd.offset[3] = 0x7fff;
  cmd.counter[0] = cmd.counter[1] = cmd.counter[2] = cmd.counter[3] = 0;

	cmd.magicno = 0x00;
	cmd.wrrom = 0;

  cmd.posneg = SET_POSNEG | CH0| CH1 | CH2 | CH3; /*POS PWM out*/
  cmd.breaks = SET_BREAKS | CH0 | CH1 | CH2 | CH3; /*No Brake*/

  if (ioctl(fd, URBTC_COUNTER_SET) < 0) report_error_and_exit("init: ioctl_COUNTER", ERR_URBTC_COUNTER_SET);
  if (write(fd, &cmd, sizeof(cmd)) < 0) report_error_and_exit("init: write_error", ERR_WRITE_CMD);
  if (ioctl(fd, URBTC_DESIRE_SET) < 0) report_error_and_exit("init: ioctl_DESIRE", ERR_URBTC_DESIRE_SET);

  for (i=0; i<4; i++) {
    obuf.ch[i].x = 0;	// x
    obuf.ch[i].d = 0; 	// v
    obuf.ch[i].kp = 1;	// propotional
    obuf.ch[i].kpx = 1;
    obuf.ch[i].kd = 10;
    obuf.ch[i].kdx = 1;
    obuf.ch[i].ki = 0;
    obuf.ch[i].kix = 10;
  }
		obuf.ch[MRIGHT].kp = -obuf.ch[MRIGHT].kp;
		obuf.ch[MRIGHT].kd = -obuf.ch[MRIGHT].kd;
		obuf.ch[MRIGHT].ki = -obuf.ch[MRIGHT].ki;
  if (write(fd, &obuf, sizeof(obuf)) < 0) report_error_and_exit("init: write_error", ERR_WRITE_OBUF);
}


void set_stat (struct mstat *mstp) {
  /* set status */
  return;
}

void print_obuf() {
  fprintf(stderr, "[ID: LEFT][x: %hd][kp: %hd][kpx: %hd][kd: %hd][kdx: %hd][ki: %hd][kix: %hd]\n",
		   obuf.ch[MLEFT].x, obuf.ch[MLEFT].kp, obuf.ch[MLEFT].kd, obuf.ch[MLEFT].kdx, obuf.ch[MLEFT].ki,
		  obuf.ch[MLEFT].kix);
  fprintf(stderr, "[ID: RIGHT][x: %hd][kp: %hd][kpx: %hd][kd: %hd][kdx: %hd][ki: %hd][kix: %hd]\n",
		   obuf.ch[MRIGHT].x, obuf.ch[MRIGHT].kp, obuf.ch[MRIGHT].kd, obuf.ch[MRIGHT].kdx, obuf.ch[MRIGHT].ki,
		  obuf.ch[MRIGHT].kix);
  return;
}

void motor_set_rot(struct mstat *mstp, short rotl, short rotr){
	/* set both right and left rotation */
	if (((-10230 < rotl) && (rotl < 10230)) && 
			((-10230 < rotr) && (rotr < 10230))) {
		// if valid input
		mstp -> rot_l = rotl;
		mstp -> rot_r = rotr;
	} else {
		fprintf(stderr, "Invalid rot input. Rot should be between -1023 and 1023\n");
	}
	return;
}

void motor_set_speed(struct mstat *mstp, short left_speed, short right_speed) {
	mstp -> spd_l = left_speed;
	mstp -> spd_r = right_speed;
	fprintf(stderr, "speedup\n");
	return;
}

int motor_write (struct mstat *mstp) {
  /* todo one of the motor should be reversed */
	obuf.ch[MRIGHT].x = (mstp -> rot_r) << 5;
	obuf.ch[MLEFT].x = (mstp -> rot_l) << 5;
  obuf.ch[MRIGHT].d = (mstp -> spd_r) << 5;
  obuf.ch[MLEFT].d = (mstp -> spd_l) << 5;
  
	if (ioctl(fd, URBTC_COUNTER_SET) < 0) report_error_and_exit("motor_write_ioctl", 4);
	if (write(fd, &cmd, sizeof(cmd)) < 0) report_error_and_exit("motor_write_cmd", 2);

	if (ioctl(fd, URBTC_DESIRE_SET) < 0) report_error_and_exit("motor_write_ioctl", 5);
	if (write(fd, &obuf, sizeof(obuf)) < 0) report_error_and_exit("motor_write_obuf", 3);

	fprintf(stderr, "wirteCMPL\n");


  return 0;
}

int is_stat (unsigned short currstat, unsigned short statbit) {
  	/* return True or False */
	return ((currstat & statbit) == statbit);
}


// TODO: implement
void
run_forward(double rot){
	motor_set_rot(&mst, rot, rot);
	motor_write(&mst);
	return;
}

// TODO: implement
void
turn_right(double angle) {
	motor_set_rot(&mst, angle, -angle);
	motor_write(&mst);
	return;
}

// TODO: implement
void
turn_left(double angle) {
	motor_set_rot(&mst, -angle, angle);
	motor_write(&mst);
	return;

}

void
set_desire(double rot[], double speed[]) {
	// index 0:left 1:right
	motor_set_rot(&mst, rot[0], rot[1]);
	motor_set_speed(&mst, speed[0], speed[1]);
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

void
report_error_and_exit(const char* msg, int rpid)
{
	perror(msg);
	exit(rpid);
}
