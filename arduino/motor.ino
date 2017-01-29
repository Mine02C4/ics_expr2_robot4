
//#include <MsTimer2.h>

String buff;
int counter = 0;
int num = 0;
int minus_flag = 0;
int num_counter = 0;
int av = 0;

void setup_rot_motor()
{
  //pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(11, OUTPUT);
  //pinMode(PIN_SPI_MISO, INPUT);
  pinMode(12, INPUT);
  //pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(13, OUTPUT);
  //pinMode(PIN_SPI_SS, OUTPUT);
  pinMode(10, OUTPUT);
  //pinMode(PIN_BUSY, INPUT);
  pinMode(9, INPUT);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  //digitalWrite(PIN_SPI_SS, HIGH);
  digitalWrite(10, HIGH);
  Serial.println("motor Set up finished.");
  L6470_resetdevice(); //L6470リセット
  L6470_setup();  //L6470を設定

  // MsTimer2::set(50, fulash);//シリアルモニター用のタイマー割り込み
  // MsTimer2::start();
  //delay(4000);
  // L6470_move(1,1600);//指定方向に指定数ステップする
  // L6470_busydelay(5000); //busyフラグがHIGHになってから、指定ミリ秒待つ。
  // L6470_run(0,10000);//指定方向に連続回転
  // delay(6000);
  // L6470_softstop();//回転停止、保持トルクあり
  // L6470_busydelay(5000);
  // L6470_goto(0x6789);//指定座標に最短でいける回転方向で移動
  //  L6470_busydelay(5000);
  // L6470_run(0,0x4567);
  // delay(6000);
  L6470_hardhiz();//回転急停止、保持トルクなし

}

void one_loop(int i) {
  Serial.println("one_loop");
  if (i == 1) {
    L6470_move(1, 25500);
  }
  if (i == 0) {
    L6470_move(0, 25500);
  }
}

void loop_arg_right(int n) {
  Serial.print("loop_arg_right ");
  Serial.println(n);
  L6470_move(1, n * 70.833);
}

void loop_arg_left(int n) {
  Serial.print("loop_arg_left ");
  Serial.println(n);
  L6470_move(0, n * 70.833);
}

void print_L6470() {
  Serial.print("0x");
  Serial.print( L6470_getparam_abspos(), HEX);
  Serial.print("  ");
  Serial.print("0x");
  Serial.println( L6470_getparam_speed(), HEX);
}


//prot
/*
  void setup_motor()
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
  // delay(1000);
  //連続運転
  //  L6470_send(0x51);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  //  L6470_send(0x00);//SPD値(20bit)
  //  L6470_send(0x40);
  //  L6470_send(0x00);
  //  delay(1000);
  L6470_send(0xB0);//SoftStop
  //  delay(1000);

  //  L6470_send(0x50);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  //  L6470_send(0x00);//値(20bit)
  // L6470_send(0x40);
  //  L6470_send(0x00);
  //  delay(1000);
  //  L6470_send(0xB0);//SoftStop
  // delay(1000);

  // L6470_send(0x50);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  // L6470_send(0x01);//値(20bit)
  // L6470_send(0x00);
  // L6470_send(0x00);
  // delay(1000);
  // L6470_send(0xB0);//SoftStop
  // delay(1000);

  // L6470_send(0x50);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  //L6470_send(0x01);//値(20bit)
  //L6470_send(0x00);
  //L6470_send(0x00);
  //delay(1000);
  // L6470_send(0xB8);//HradStop
  // delay(1000);

  L6470_send(0x70);//GoHome
  //delay(1000);
  Serial.println("SET UP");
  }



  void loop_motor()
  {
  Serial.println("loop start");
  L6470_send(0x51);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  L6470_send(0x02);//SPD値(20bit)
  L6470_send(0x51);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  L6470_send(0x02);//SPD値(20bit)
  L6470_send(0x51);//Run(DIR,SPD),0x51:正転,0x50:逆転　
  L6470_send(0x02);//SPD値(20bit)
  //    L6470_send(0x40);
  //  L6470_send(0x00);
  // delay(1000);
  // L6470_send(0xB0);//SoftStop
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
  }*/
