#include <Servo.h>

Servo servo;

void setup()
{
	servo.attach(9);
}

void loop()
{
	servo.write(10);
	delay(10000);
	servo.write(10);
	delay(10000);
}
