#include "Arduino.h"
#include "my_wifi.h"
#include "my_baidu_ts.h"
#include "my_deepseek.h"

// ------对象定义------
Audio audio;
My_I2S I2S_1;
My_GPT My_deepseek;

String GPTinputText = "你好，深度求索！";
String Init_encodedText = "你好，我是量子位，你的专业果林助手，需要什么帮助吗？";
String new_encodedText = "0", Init_url = "0", GPTanswer = "0";
// 3、修改百度语音助手的用户信息
// 4、修改播放文本内容

// ------变量声明------
uint16_t audioBuffer[bufferlen];
uint8_t Key_State = 0;

// ------函数声明------
void player(String url);    

void setup() {
  Serial.begin(115200);
  WiFi_Setup();
  I2S_1.my_inmp441_init();
  pinMode(48, INPUT);
  //Init_encodedText = urlEncode(urlEncode(Init_encodedText));
  String stt_tts_setup();
  Init_url = tts_get(Init_encodedText);
  player(Init_url);
  GPTanswer = My_deepseek.getGPTAnswer(GPTinputText);
  Serial.println("Answer: " + GPTanswer);
  Serial.println("Enter a prompt:");
}

void loop() {
  audio.loop();
  Key_State = digitalRead(48);
  
  if (Serial.available()) { 
    //  1. 若串口有内容输入，先停止语音播报
    audio.stopSong();
    GPTinputText = Serial.readStringUntil('\r');//  \r表示结束符为回车符
    Serial.println("\n Input:" + GPTinputText);
    GPTanswer = My_deepseek.getGPTAnswer(GPTinputText);
    new_encodedText = urlEncode(GPTanswer);
    String url = tts_get(new_encodedText);
    Serial.println("Answer: " + GPTanswer);
    Serial.println("Enter a prompt:");
    audio.connecttohost(url.c_str());
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
void player(String url) {
  const char *host = url.c_str();
  audio.setPinout(MAX98357_BCLK, MAX98357_LRC, MAX98357_DIN);
  audio.setVolume(3);        // 0...21
  audio.connecttohost(host);  //  128k mp3
}

