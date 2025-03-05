#include <Arduino.h>
#include "my_wifi.h"
#include "my_baidu_ts.h"
#include "my_deepseek.h"
#include "TJC.h"

// ------对象定义------
Audio audio;
My_GPT My_deepseek;

String GPTinputText = "你好，深度求索！";
String Init_encodedText = "你好，我是量子位，你的专业果林助手，需要什么帮助吗？";
String new_encodedText = "0", Init_url = "0", GPTanswer = "0";

// ------变量声明------
uint16_t audioBuffer[bufferlen];
uint32_t delayMS;
unsigned long nowtime;
uint8_t Key_State = 0;

// ------函数声明------
void max98357a_player(String url);          // 扬声器播放音频    

void setup() {
  // 设备初始化
  Serial.begin(115200);
  TJC.begin(115200, SERIAL_8N1, TJC_RX, TJC_TX);  //串口1初始化
  Wire.setPins(SDA,SCL);
  Wire.begin();
  WiFi_Setup();
  pinMode(48, INPUT);
  
  // dht11初始化
  dht.begin();
  analogReadResolution(12);  // 设置ADC分辨率位12位
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  //  BH1750初始化
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));

  //因为串口屏开机会发送88 ff ff ff,所以要清空串口缓冲区
  while (TJC.read() >= 0); //清空串口缓冲区
  TJC.print("page start\xff\xff\xff"); //发送命令让屏幕跳转到main页面
  nowtime = millis(); //获取当前已经运行的时间
  delayMS = sensor.min_delay / 1000;

  // 语音播报初始化
  //Init_encodedText = urlEncode(urlEncode(Init_encodedText));
  String stt_tts_setup();                   // 获取STT和TTS的token 
  Init_url = tts_get(Init_encodedText);     // 获取encoede编码后的url
  max98357a_player(Init_url);
  GPTanswer = My_deepseek.getGPTAnswer(GPTinputText);
  Serial.println("Answer: " + GPTanswer);
  Serial.println("Enter a prompt:");
}

void loop() {
  audio.loop();
  Key_State = digitalRead(48);
  //  1s更新一次传感器数据（模仿定时器中断）
  if (millis() >= nowtime + 1000) {
    nowtime = millis(); //获取当前已经运行的时间
    TJC_Sensor();
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
  }

  // 若串口屏输入则控制LED
  while(TJC.available() >= FRAME_LENGTH){
    TJC_Light();
  }

  // 若电脑串口输入则控制LLM
  if (Serial.available()) { 
    //  1. 若串口有内容输入，先停止语音播报
    audio.stopSong();
    GPTinputText = Serial.readStringUntil('\r');//  \r表示结束符为回车符
    Serial.println("\n Input:" + GPTinputText);
    GPTanswer = My_deepseek.getGPTAnswer(GPTinputText);   // 获取GPT回答
    new_encodedText = urlEncode(GPTanswer);
    String url = tts_get(new_encodedText);                // 获取TTS音频url
    Serial.println("Answer: " + GPTanswer);
    Serial.println("Enter a prompt:");
    audio.connecttohost(url.c_str());                     // 播放新获得的音频
  }
  log_i("free heap=%i", ESP.getFreeHeap());
}

/**
 * @brief Function for 播放音频.
 *
 * @param[in] void.
 * 
 * @return (String)经过TTS处理后返回的音频url.
 * 
 **/
void max98357a_player(String url) {
  const char *host = url.c_str();
  audio.setPinout(MAX98357_BCLK, MAX98357_LRC, MAX98357_DIN);
  audio.setVolume(5);        // 0...21
  audio.connecttohost(host);  //  128k mp3
}

