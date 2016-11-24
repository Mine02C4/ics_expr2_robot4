#include <SPI.h>
 
// ピン定義。
//#define 11 11
//#define 12 12
//#define 13 13
//#define 10 10
 
void setup()
{
 
  delay(2000);
  // ピン設定。
  pinMode(11, OUTPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  //SPI通信開始
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);//SCKの立ち上がりでテータを送受信＆アイドル時はpinをHIGHに設定
  SPI.setBitOrder(MSBFIRST);//MSBから送信
 Serial.begin(9600);
  Serial.println("set up");
  //前のコマンドの引数を消去
  L6470_send(0x00);//nop
  L6470_send(0x00);
  L6470_send(0x00);
  L6470_send(0x00);
  //デバイスリセットコマンド
  L6470_send(0xc0);//ResetRevice
 
  L6470_setup();//L6470を設定
 Serial.println("l6470 set up called");
  delay(1000);
  //連続運転
  L6470_send(0x51);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  L6470_send(0x00);//SPD値(20bit)
  L6470_send(0x40);
  L6470_send(0x00);
  delay(1000);
  L6470_send(0xB0);//SoftStop
  delay(1000);
 
  L6470_send(0x50);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  L6470_send(0x00);//値(20bit)
  L6470_send(0x40);
  L6470_send(0x00);
  delay(1000);
  L6470_send(0xB0);//SoftStop
  delay(1000);
 
  L6470_send(0x50);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  L6470_send(0x01);//値(20bit)
  L6470_send(0x00);
  L6470_send(0x00);
  delay(1000);
  L6470_send(0xB0);//SoftStop
  delay(1000);
 
  L6470_send(0x50);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  L6470_send(0x01);//値(20bit)
  L6470_send(0x00);
  L6470_send(0x00);
  delay(1000);
  L6470_send(0xB8);//HradStop
  delay(1000);
 
  L6470_send(0x70);//GoHome
  delay(1000);
   Serial.println("SET UP");
}
 
 
 
void loop()
{
   Serial.println("loop start");
   L6470_send(0x51);//Run(DIR,SPD),0x51:正転,0x50:逆転　
   L6470_send(0x00);//SPD値(20bit)
   L6470_send(0x51);//Run(DIR,SPD),0x51:正転,0x50:逆転　
   L6470_send(0x00);//SPD値(20bit)
   L6470_send(0x51);//Run(DIR,SPD),0x51:正転,0x50:逆転　
   L6470_send(0x00);//SPD値(20bit)
//    L6470_send(0x40);
//  L6470_send(0x00);
 // delay(1000);
  L6470_send(0xB0);//SoftStop
//delay(1000);
  Serial.println("loop");
}
 
 
void L6470_send(unsigned char add_or_val){
   Serial.println("send");

  digitalWrite(10, LOW); 
  SPI.transfer(add_or_val); // アドレスもしくはデータ送信。
  digitalWrite(10, HIGH); 
}
 
void L6470_setup(){
  Serial.println("l6470 set up start");
  //最大回転スピード
  L6470_send(0x07);//レジスタアドレス
  L6470_send(0x00);//値(10bit),デフォルト0x41
  L6470_send(0x30);
  //モータ停止中の電圧設定
  L6470_send(0x09);//レジスタアドレス
  L6470_send(0x20);//値(8bit),デフォルト0x29
  //モータ定速回転時の電圧設定
  L6470_send(0x0a);//レジスタアドレス
  L6470_send(0x40);//値(8bit),デフォルト0x29
  //加速中の電圧設定
  L6470_send(0x0b);//レジスタアドレス
  L6470_send(0x40);//値(8bit),デフォルト0x29
  //減速中の電圧設定
  L6470_send(0x0c);//レジスタアドレス
  L6470_send(0x40);//値(8bit),デフォルト0x29
 
  //フ ル ス テ ッ プ,ハ ー フ ス テ ッ プ,1/4, 1/8,…,1/128 ステップの設定
  L6470_send(0x16);//レジスタアドレス
  L6470_send(0x00);//値(8bit)
 Serial.println("l6470 set up end");
}
