void setup_serial()
{  
  Serial.begin(9600);
}

void loop_motor() {
  int tmp;
  if (Serial.available() > 0) {

    char data = Serial.read();
    Serial.println("data:");
    Serial.println(data);
    //Serial.println(data);
    if (data == 'x') {
      //Serial.println("get x");

      //buff[0]～buff[counter-1]までが文字列となってここでうけとれる
      //シリアル送信側で終端文字\nが最後につけられることが前提
      //Serial.println("new line\n");

      if (buff.equalsIgnoreCase("fire") || buff.equalsIgnoreCase("\nfire")) {
        Serial.println("fire command");
        loop_gun();
      }
      else if ((buff.equalsIgnoreCase("fire") || buff.equalsIgnoreCase("\nfire") ) && num_counter != 0) {
        loop_gun_n(num);
        Serial.println("fire=n command");
      }
      else if (buff.equalsIgnoreCase("turn") || buff.equalsIgnoreCase("\nturn")) {
        Serial.println("turn command");
        if (minus_flag == 1) {
          tmp = -num;
          num = -num - av;
          av = tmp;
          if (num < 0)
            loop_arg_right(-num);
          else
            loop_arg_left(num);
        }
        else {
          tmp = num;
          num = num - av;
          av = tmp;
          if (num < 0) {
            loop_arg_right(-num);

          }
          else {
            loop_arg_left(num);

          }

        }
        // Serial.println("turning");
        delay(5000);
      }
      else if (buff.equalsIgnoreCase("turret") || buff.equalsIgnoreCase("\nturret")) {
        //27is min
        Serial.println("turret command");
        if (minus_flag == 1)
          num = -num;
        servo.write(num + 50);
      }
      Serial.println("buff:");
      Serial.println(buff);
      Serial.println("fire:sample");
      Serial.println("av = ");
      Serial.println(av);
      Serial.println("num = ");
      Serial.println(num);

      counter = 0;
      num_counter = 0;
      num = 0;
      minus_flag = 0;
      buff = "";
      Serial.println("buff:");
      Serial.println(buff);
    }

    else if (data == '=') {
      num_counter++;

    }
    else if (num_counter != 0) {
      if (num_counter == 1) {
        if (data == '-')
          minus_flag = 1;
        else
          num = data - '0';
        num_counter++;
      }
      else {
        if (minus_flag == 1 && num_counter == 1)
          num = data - '0';
        else
          num = num * 10 + data - '0';
        num_counter++;
      }
    }
    else {
      buff += data;
    }
  }

}
