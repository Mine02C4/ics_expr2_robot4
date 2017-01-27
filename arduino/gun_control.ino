void setup_gun() {
  pinMode(7,OUTPUT); 
  Serial.begin(9600);
  Serial.println("gun set up finished");
}

void loop_gun() {
  digitalWrite(7,HIGH);
  delay(150);
  
  digitalWrite(7,LOW); 
  Serial.println("gun loop");
}

void loop_gun_n(int n) {
  digitalWrite(7,HIGH);
  delay(150 * n);
  digitalWrite(7,LOW); 
  }

void gun_burst (int n) {
  int i = 0;
  for (;i < n; i++){
    digitalWrite(7,HIGH);
    delay(150 * 3);
    digitalWrite(7,LOW);  
    delay(1000);
  } 
 }

void test(){
  digitalWrite(7,HIGH);
}

