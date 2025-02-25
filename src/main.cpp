#include <Audio.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Arduino.h"
//1. Replace with your network credentials
const char* ssid = "Redmi Note 12 Turbo";
const char* password = "12345678";      // Change this to your WiFi password
String voice_id = "female-tianmei-jingpin";  //青年大学生音色：male-qn-daxuesheng;甜美女性音色：female-tianmei;男性主持人：presenter_male;女性主持人：presenter_female
// 2. Replace with your OpenAI API key
const char* apiKey = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJHcm91cE5hbWUiOiLph4_lrZDkvY0iLCJVc2VyTmFtZSI6IumHj-WtkOS9jSIsIkFjY291bnQiOiIiLCJTdWJqZWN0SUQiOiIxODkzODYxMDI1MDc0MTIzMTEyIiwiUGhvbmUiOiIxMzA2ODgyNDgxNyIsIkdyb3VwSUQiOiIxODkzODYxMDI1MDY1NzM0NTA0IiwiUGFnZU5hbWUiOiIiLCJNYWlsIjoiIiwiQ3JlYXRlVGltZSI6IjIwMjUtMDItMjUgMTA6MTA6NTQiLCJUb2tlblR5cGUiOjEsImlzcyI6Im1pbmltYXgifQ.aQKisxDl9i01vf-PB1f73j2tMSraOtv8IhA13jUKqr-go9xLda_XFTdSHSp-w1CB_m2_zKA2rcuK4gOPyijuLBFXjsdHEpKLXwsfKCU7O4b1R6IOpPDe1f3EV8IgQuuOtPuId_rejQ3ArUuGjuKc1vttu8zzbCgx3aDrorlE8oK_9h6DgqVKU_SVV8ESWciLx-tZQ0u0LeX1C58f4B4ZeUsbX-Mn2LXug-k0A_6rhEm20aOy561OBcZ6dnO1UYnWF1IYz--ol-WY1Ibx4E4MtROnq2FToxASH9vmJqKKHH0B4bo_Ht-8nKDmBslK2_LAPDIsIQkcWR40ixG738OTag";
const char* group_id = "1893861025065734504";
const char* url = "https://api.minimax.chat/v1/t2a_pro?GroupId=1893861025065734504";
char myCharPointer;
//扬声器引脚
#define I2S_DOUT 13         // DIN connection
#define I2S_BCLK 2          // Bit clock
#define I2S_LRC  15         // Left Right Clock

Audio audio;
String answerv;
String getvAnswer(void) {
  HTTPClient http1;
  http1.setTimeout(10000);
  http1.begin(url);
  http1.addHeader("Content-Type", "application/json");
  String token_key = String("Bearer ") + apiKey;
  http1.addHeader("Authorization", token_key);

  // 创建一个StaticJsonDocument对象，足够大以存储JSON数据
  StaticJsonDocument<200> doc;

  // 填充数据
  doc["text"] = "我是智能助手，你好呀";
  doc["model"] = "speech-01";
  doc["audio_sample_rate"] = 32000;
  doc["bitrate"] = 128000;
  doc["voice_id"] = voice_id;

  // 创建一个String对象来存储序列化后的JSON字符串
  String jsonString;

  // 序列化JSON到String对象
  serializeJson(doc, jsonString);
  int httpResponseCode1 = http1.POST(jsonString);

  if (httpResponseCode1 != 200) {
    Serial.println("HTTP Request Failed");
    http1.end();

    return "<error>";
  }

  String reason = http1.getString();
  Serial.println("Received response:");
  Serial.println(reason);
  http1.end();
  DynamicJsonDocument jsonDoc1(1024);
  deserializeJson(jsonDoc1, reason);
  String outputText = jsonDoc1["audio_file"];
  return outputText;
}

void set_voice() {
  answerv = getvAnswer();
  Serial.print(answerv);
  char myCharPointer[answerv.length() + 1];  // 分配足够的空间来存储字符串
  strcpy(myCharPointer, answerv.c_str());    // 复制字符串到 myCharPointer
  audio.connecttohost(myCharPointer);        //  128k mp3
}


void setup() {
  // Initialize Serial
  Serial.begin(115200);
  // Connect to Wi-Fi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());



  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(20);  // 0...21
  // audio.connecttohost("https://minimax-algeng-chat-tts.oss-cn-wulanchabu.aliyuncs.com/audio%2Ftts-mp3-20240406123922-amZgimLG.mp3?Expires=1712493562&OSSAccessKeyId=LTAI5tGLnRTkBjLuYPjNcKQ8&Signature=omJ5fmpPvXMoR1nk9D8UbJqR3L4%3D"); //  128k mp3
}

void loop() {

  audio.loop();
  while (Serial.available() > 0) {
    char voice = Serial.read();
    // Serial.println(voice);
    switch (voice) {
      case '1':
        voice_id = "male-qn-daxuesheng";
        break;
      case '2':
        voice_id = "female-tianmei";
        break;
      case '3':
        voice_id = "presenter_male";
        break;
      case '4':
        voice_id = "presenter_female";
        break;
    }
    Serial.println(voice_id);
    set_voice();
  }
  
}


