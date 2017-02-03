#include <Servo.h>

//Servo servo;

int servo_pos;

void setup_servo()
{
  delay(2000);
	servo.attach(9);
  servo_pos = servo.read();  
  servo.write(50);

  Serial.println("servo set up finished");
  Serial.println(servo_pos);
}

void loop_servo()
{
/*
  servo.write(0);
  delay(1000);
  servo.write(30);
  delay(1000);
  
  */
 Serial.println("servo loop");
}
