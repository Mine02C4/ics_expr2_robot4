#include <SPI.h>
#include <Servo.h>
Servo servo;

void setup() {
 setup_gun();
 setup_motor();
 //setup_servo();
 Serial.println("All set up finished");
}

void loop() {

 loop_motor();
//loop_servo();
//loop_gun() ;
  
}
