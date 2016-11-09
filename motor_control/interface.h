#ifndef MOTOR_CTRL_INTERFACE_H_
#define MOTOR_CTRL_INTERFACE_H_

// set motor channel
#define MRIGHT 2
#define MLEFT 3

// motor status
#define STAT_DEFAULT 0x0000 // default
#define STAT_STPG 0x0001  // stopping
#define STAT_STPD 0x0002    // stopped
#define STAT_MVF 0x0004   // moving fwd
#define STAT_MVB 0x0008   // moving bwd
#define STAT_DEC 0x0010   // deacceralating
#define STAT_ACC 0x0020   // acceralating
#define STAT_TR 0x0040    // turning right
#define STAT_TL 0x0080    // turning left
#define STAT_ERR 0x8000   // error

extern void motor_init();
extern void motor_test_loop();
extern void motor_exit_loop(); // MUST CALL before closing program!!!
extern void motor_finalize(); // MUST CALL before closing program!!!
extern void motor_set_by_double(double);

enum wheel { LEFT, RIGHT };

struct mstat {
  unsigned short stat;
  unsigned short rstat;
  unsigned short lstat;
  /* +- 0 - 1024 */
  short lm, rm; // left motor
};
#endif  // MOTOR_CTRL_INTERFACE_H_
