#ifndef MOTOR_CTRL_INTERFACE_H_
#define MOTOR_CTRL_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

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

// errors
#define ERR_URBTC_DESIRE_SET 1
#define ERR_WRITE_CMD 2
#define ERR_URBTC_COUNTER_SET 3
#define ERR_WRITE_OBUF 4
#define ERR_URBTC_COUTINUOUS_READ 5

struct mstat;
enum wheel { LEFT, RIGHT };

/* Init API */
extern void motor_init();

/* Control API */
extern void run_forward(double seconds);
extern void turn_right(double seconds);
extern void turn_left(double seconds);

/* Finalize API */
extern void motor_finalize(); // MUST CALL before closing program!!!

/* Not API */
extern void print_obuf();
extern void mstat_init(struct mstat *);
extern void motor_test_loop();
extern void motor_exit_loop(); // MUST CALL before closing program!!!
extern int motor_write(struct mstat *);
extern void motor_set_rot(struct mstat *, short, short);
extern void motor_set_speed(struct mstat *, short, short);

struct mstat {
  unsigned short stat;
  unsigned short rstat;
  unsigned short lstat;
  
  /* -1023 to +1023 */
  short rot_l, rot_r; // left motor right motor
	short spd_l, spd_r;
};

#ifdef __cplusplus
}
#endif

#endif  // MOTOR_CTRL_INTERFACE_H_

