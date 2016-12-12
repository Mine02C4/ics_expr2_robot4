//#include <SPI.h>
void setup_gun() {
  // put your setup code here, to run once:
pinMode(7,OUTPUT); 
Serial.begin(9600);
   delay(10000);
Serial.println("set up finished");

// motor

}

void loop_gun() {
 
 /* Serial.println("5");
    delay(1000); Serial.println("4");
    delay(1000); Serial.println("3");
    delay(1000); Serial.println("2");
    delay(1000); Serial.println("1");
    delay(1000);*/
  // put your main code here, to run repeatedly:
digitalWrite(7,HIGH);//  Serial.println("high");

  delay(150);
  digitalWrite(7,LOW); // Serial.println("low");
    //delay(1000);
}

void loop_gun_n(int n) {
   Serial.println("start");
    delay(1000);
  //    Serial.println("10");
   // delay(1000); Serial.println("9");
  //  delay(1000); Serial.println("8");
//    delay(1000); Serial.println("7");
//    delay(1000); Serial.println("6");
//    delay(1000);
 /* Serial.println("5");
    delay(1000); Serial.println("4");
    delay(1000); Serial.println("3");
    delay(1000); Serial.println("2");
    delay(1000); Serial.println("1");
    delay(1000);*/
  // put your main code here, to run repeatedly:
digitalWrite(7,HIGH);//  Serial.println("high");

  delay(150 * n);
  digitalWrite(7,LOW); // Serial.println("low");
    //delay(1000);
}
