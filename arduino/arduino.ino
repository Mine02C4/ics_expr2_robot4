#include <SPI.h>
void setup() {
 

  // put your setup code here, to run once:
  setup_gun();
  setup_motor();
}

void loop() {
  // put your main code here, to run repeatedly:
 // loop_gun();

  loop_motor();

  
}
