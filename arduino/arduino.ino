#include <SPI.h>
#include <Servo.h>

Servo servo;

void setup() {
  setup_serial();
  setup_gun();
  setup_rot_motor();
  setup_servo();
  Serial.println("All set up finished");
}

void loop() {

  loop_motor();


}
