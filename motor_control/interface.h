#ifndef MOTOR_CTRL_INTERFACE_H_
#define MOTOR_CTRL_INTERFACE_H_

extern void motor_init();
extern void motor_test_loop();
extern void motor_exit(); // MUST CALL before close program!!!

#endif  // MOTOR_CTRL_INTERFACE_H_
