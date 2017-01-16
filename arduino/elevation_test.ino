#include <Servo.h>

//Servo servo;

void setup_servo()
{
	servo.attach(9);
  Serial.println("servo set up finished");
}

void loop_servo()
{
	//servo.write(10);
 // servo.write(-10);
  //delay(1000);
  servo.write(0);
  delay(1000);
  servo.write(30);
  delay(1000);
  
  
 Serial.println("servo loop");
}
