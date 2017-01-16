#include <SPI.h>
#include <Servo.h>
Servo servo;

void setup() {
 setup_gun();
 setup_motor();
 setup_servo();
}

void loop() {
 //test();
 loop_motor();
loop_servo();
  
}
