#ifndef MOTOR_CTRL_INTERFACE_H_
#define MOTOR_CTRL_INTERFACE_H_

extern void motor_init();
extern void motor_test_loop();
extern void motor_exit_loop(); // MUST CALL before close program!!!
extern void motor_finalize(); // MUST CALL before close program!!!
extern void motor_set_by_double(double);

#endif  // MOTOR_CTRL_INTERFACE_H_
