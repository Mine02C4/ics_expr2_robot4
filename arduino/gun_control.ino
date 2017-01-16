void setup_gun() {
  pinMode(7,OUTPUT); 
  Serial.begin(9600);
  delay(10000);
  Serial.println("gun set up finished");
}

void loop_gun() {
  digitalWrite(7,HIGH);
  delay(1500);
  
  digitalWrite(7,LOW); 
  Serial.println("gun loop");
}

void loop_gun_n(int n) {
  digitalWrite(7,HIGH);
  delay(150 * n);
  digitalWrite(7,LOW); 
  }

void test(){
  digitalWrite(7,HIGH);
}

