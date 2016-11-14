#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

// set motor channel
#define MRIGHT 0
#define MLEFT 2

#include "../imcs01_driver_JROBO/driver/urbtc.h"
#include "../imcs01_driver_JROBO/driver/urobotc.h"

#include "interface.h"

const static char *devfile = "/dev/urbtc0";

static struct uin ibuf;
static struct uout obuf;
static struct ccmd cmd;
static int fd, fds;

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
#if __BYTE_ORDER == __BIG_ENDIAN
  ibuf.magicno = (0xff & ibuf.magicno)<<8 | (0xff00 & ibuf.magicno)>>8;
#endif
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

  cmd.selin = CH0 | CH1 | SET_SELECT; /* AD in:ch0,ch1    ENC in:ch2,ch3*/
  cmd.selout = SET_SELECT | CH0 | CH1 | CH2 | CH3; /*  PWM out:ch0,ch1,ch2,ch3*/

#if __BYTE_ORDER == __LITTLE_ENDIAN
  cmd.offset[0] = cmd.offset[1] = cmd.offset[2] = cmd.offset[3] = 0x7fff;
  cmd.counter[0] = cmd.counter[1] = cmd.counter[2] = cmd.counter[3] = 0;
#else
  cmd.offset[0] = cmd.offset[1] = cmd.offset[2] = cmd.offset[3] = 0xff7f;
  cmd.counter[0] = cmd.counter[1] = cmd.counter[2] = cmd.counter[3] = 0;
#endif

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
#if __BYTE_ORDER == __LITTLE_ENDIAN
    obuf.ch[i].x = 0;
    obuf.ch[i].d = 0;
    obuf.ch[i].kp = 0;
    obuf.ch[i].kpx = 1;
    obuf.ch[i].kd = 0;
    obuf.ch[i].kdx = 1;
    obuf.ch[i].ki = 0;
    obuf.ch[i].kix = 1;
#else
    obuf.ch[i].x = 0;
    obuf.ch[i].d = 0;
    obuf.ch[i].kp = 0;
    obuf.ch[i].kpx = 0x0100;
    obuf.ch[i].kd = 0;
    obuf.ch[i].kdx = 0x0100;
    obuf.ch[i].ki = 0;
    obuf.ch[i].kix = 0x0100;
#endif
  }
#if __BYTE_ORDER == __LITTLE_ENDIAN
  obuf.ch[2].kp = 0x10;
  obuf.ch[3].kp = 0x10;
#else
  obuf.ch[2].kp = 0x1000;
  obuf.ch[3].kp = 0x1000;
#endif
}

void
motor_test_loop()
{
  int i = 0;
  while(motor_quit_flag) {
    unsigned short a = 300.0*sin(i*3.14/655.360);// + 512.0;
    a <<= 5;

#if __BYTE_ORDER == __LITTLE_ENDIAN
    obuf.ch[2].x = obuf.ch[3].x = a;
#else
    obuf.ch[2].x = obuf.ch[3].x = ((a & 0xff) << 8 | (a & 0xff00) >> 8);
#endif
    printf("%x\r\n",obuf.ch[3].x);

    if (write(fds, &obuf, sizeof(obuf)) > 0) {
      i++;
    } else {
      printf("write err\n");
      break;
    }
  }
  close(fds);
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

void motor_set(struct mstat *mstp, short rotl, short rotr){
	/* set both right and left rotation */
	if (((-1023 < rotl) && (rotl < 1023)) && 
			((-1023 < rotr) && (rotr < 1023))) {
		// if valid input
		mstp -> lstat = rotl;
		mstp -> rstat = rotr;
	} else {
		fprintf(stderr, "Invalid rot input. Rot should be between -1023 and 1023\n");
	}
	return;
}

int motor_write (struct mstat *statp) {
  /* todo one of the motor should be reversed */
  short rotl = statp -> motor_l; // left motor rotation
  short rotr = statp -> motor_r; // 512
#if __BYTE_ORDER == __LITTLE_ENDIAN
  obuf.ch[MRIGHT].x = rotr; // set right rounds
  obuf.ch[MLEFT].x = rotl + 1024; // set left rounds
#else
  obuf.ch[MRIGHT].x = ((rotr & 0xff) << 8 | (rotr & 0xff00) >> 8);
  obuf.ch[MLEFT].x = ((rotl & 0xff) << 8 | (rotl & 0xff00) >> 8);
#endif
  if (write(fds, &obuf, sizeof(obuf)) > 0) {
    printf("MRIGHT: %x MLEFT: %x\r\n", 
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

void
motor_set_by_double(double val)
{
  /* set mortor  by double */
  unsigned short a = val * 300.0;// + 512.0;
  a <<= 6;
#if __BYTE_ORDER == __LITTLE_ENDIAN
  obuf.ch[2].x = obuf.ch[3].x = a;
#else
  obuf.ch[2].x = obuf.ch[3].x = ((a & 0xff) << 8 | (a & 0xff00) >> 8);
#endif
  printf("%x\r\n",obuf.ch[3].x);
  if (write(fds, &obuf, sizeof(obuf)) > 0) {
  } else {
    printf("write err\n");
  }
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

