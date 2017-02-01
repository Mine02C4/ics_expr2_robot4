#include <Servo.h>

//Servo servo;

int servo_pos;

void setup_servo()
{
  delay(2000);
	servo.attach(9);
  servo_pos = servo.read();
   Serial.println("pos read\n");
  
  int i = servo_pos - 50;
  while(i < 0){
    delay(10);
    servo.write(50 + i);
    i--;
  }
   Serial.println("first while finished");
  
  while(i > 0){
    delay(10);
    servo.write(50 + i);
    i++;
  }
  
   Serial.println("socond while finished");
  
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
