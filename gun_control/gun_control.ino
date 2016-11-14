//#include <SPI.h>
void setup() {
  // put your setup code here, to run once:
pinMode(11,OUTPUT); 
Serial.begin(9600);
   delay(10000);
Serial.println("set up finished");
}

void loop() {
   Serial.println("start");
    delay(1000);
      Serial.println("10");
    delay(1000); Serial.println("9");
    delay(1000); Serial.println("8");
    delay(1000); Serial.println("7");
    delay(1000); Serial.println("6");
    delay(1000);
  Serial.println("5");
    delay(1000); Serial.println("4");
    delay(1000); Serial.println("3");
    delay(1000); Serial.println("2");
    delay(1000); Serial.println("1");
    delay(1000);
  // put your main code here, to run repeatedly:
digitalWrite(11,HIGH);//  Serial.println("high");

  delay(150);
  digitalWrite(11,LOW); // Serial.println("low");
    delay(1000);
}
