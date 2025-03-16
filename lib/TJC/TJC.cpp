#include "TJC.h"

DHT_Unified dht(DHTPIN, DHTTYPE);
MQ135 mq135_sensor(PIN_MQ135);
BH1750 lightMeter;

float temperature = 21.0, humidity = 25.0;
char str[100];      //定义一个字符串数组，用于存放传感器数据要发送的字符串
uint8_t Servo_Angle[4] = {90, 90, 90, 90};

void TJC_Sensor(){
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  sprintf(str, "n0.val=%d\xff\xff\xff", (uint8_t)event.temperature);
  TJC.print(str);
  dht.humidity().getEvent(&event);
  sprintf(str, "n1.val=%d\xff\xff\xff", (uint8_t)event.relative_humidity);
  TJC.print(str);
  float lux = lightMeter.readLightLevel();
  sprintf(str, "n3.val=%d\xff\xff\xff", (uint16_t)lux);
  TJC.print(str);
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
  sprintf(str, "n4.val=%d\xff\xff\xff", (uint16_t)correctedPPM);
  TJC.print(str);
  String t5_txt = Soil_judge();
  sprintf(str, "t5.txt=\"%s\"\xff\xff\xff", t5_txt);
  TJC.print(str);
  delay(50);

    //用sprintf来格式化字符串，触发b0的弹起事件,直接把结束符整合在字符串中
    //sprintf(str, "click b0,0\xff\xff\xff");
    //把字符串发送出去
    //TJC.print(str);
}


  //串口数据格式：
  //串口数据帧长度：7字节
  //帧头     参数1    参数2   参数3       帧尾
  //0x55     1字节   1字节    1字节     0xffffff
  //当参数是01时
  //帧头     参数1    参数2   参数3       帧尾
  //0x55     01     led编号  led状态    0xffffff
  //例子1：上位机代码  printh 55 01 01 00 ff ff ff  含义：1号led关闭
  //例子2：上位机代码  printh 55 01 04 01 ff ff ff  含义：4号led打开
  //例子3：上位机代码  printh 55 01 00 01 ff ff ff  含义：0号led打开
  //例子4：上位机代码  printh 55 01 04 00 ff ff ff  含义：4号led关闭

  //当参数是02或03时
  //帧头     参数1    参数2      参数3       帧尾
  //0x55     02/03   水果品种    00    0xffffff
  //例子1：上位机代码  printh 55 02 01 00 00 00 ff ff ff  含义：要驱动苹果坐标
  //例子2：上位机代码  printh 55 02 00 00 ff ff ff  含义：h0.val=0

  //当参数是04时,下发的是机械臂舵机的角度信息
  //帧头     参数1    参数2   参数3   参数4   参数5     帧尾
  //0x55     04      00      00      00      00       0xffffff
  //例子1：上位机代码  printh 55 04 00 00 00 00 ff ff ff  含义：舵机角度没有变化

void TJC_Light(){
    unsigned char ubuffer[FRAME_LENGTH];
    //从串口缓冲读取1个字节但不删除
    unsigned char frame_header = TJC.peek();
    //当获取的数据是包头(0x55)时
    if (frame_header == 0x55) {
      //从串口缓冲区读取7字节
      TJC.readBytes(ubuffer, FRAME_LENGTH);
      for(int i = 0; i < FRAME_LENGTH; i++)
      {
        Serial.print(ubuffer[i], HEX);
        Serial.print(" ");
      }
      Serial.println("");
      if (ubuffer[6] == 0xff && ubuffer[7] == 0xff && ubuffer[8] == 0xff) {
        if(ubuffer[1] == 0x01)
        {
          //下发的是LED信息
          sprintf(str, "msg.txt=\"led %d is %s\"\xff\xff\xff", ubuffer[2], ubuffer[3] ? "on" : "off");
          TJC.print(str);
          if(ubuffer[3] == 0x01){
            digitalWrite(LED_Pin, HIGH);
          }else if(ubuffer[3] == 0x00){
            digitalWrite(LED_Pin, LOW);
          }
        }else if(ubuffer[1] == 0x02)
        {
          //下发的是水果信息
          if(ubuffer[2] == 0x01)  My_Apple_Move();
        }else if(ubuffer[1] == 0x04)
        {
          if(ubuffer[2] != 0x00)  ubuffer[2] == 0x01 ? Servo_Angle[0] += Servo_Step : Servo_Angle[0] -= Servo_Step;
          if(ubuffer[3] != 0x00)  ubuffer[3] == 0x01 ? Servo_Angle[1] += Servo_Step : Servo_Angle[1] -= Servo_Step;
          if(ubuffer[4] != 0x00)  ubuffer[4] == 0x01 ? Servo_Angle[2] += Servo_Step : Servo_Angle[2] -= Servo_Step;
          if(ubuffer[5] != 0x00)  ubuffer[5] == 0x01 ? Servo_Angle[3] += Servo_Step : Servo_Angle[3] -= Servo_Step;
          My_Servo_Move(Servo_Angle);
          Serial.printf("Servo_Angle = %d %d %d %d\n", Servo_Angle[0], Servo_Angle[1], Servo_Angle[2], Servo_Angle[3]);
          sprintf(str, "msg.txt=\"Servo Angle is %d %d %d %d\"\xff\xff\xff", Servo_Angle[0], Servo_Angle[1], Servo_Angle[2], Servo_Angle[3]);
        }
      }
    } else {
      TJC.read();  //从串口缓冲读取1个字节并删除
    }
}
