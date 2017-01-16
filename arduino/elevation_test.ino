#include <Servo.h>

//Servo servo;

void setup_servo()
{
	servo.attach(9);
}

void loop_servo()
{
	servo.write(10);
	delay(10000);
	servo.write(10);
	delay(10000);
}
