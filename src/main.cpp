#include "Arduino.h"
#include "my_wifi.h"
#include "my_baidu_ts.h"

// 1、修改MAX98357喇叭接口
Audio audio;

String Init_encodedText = "你好，我是量子位，你的专业果林助手，需要什么帮助吗？";
String new_encodedText = "0",Init_url = "0";
// 2、修改WiFi密码
// 3、修改百度语音助手的用户信息
// 4、修改播放文本内容

void player(String url);

void setup() {
  Serial.begin(115200);
  WiFi_Setup();
  void my_inmp441_init();
  void my_max98357a_init();
  //Init_encodedText = urlEncode(urlEncode(Init_encodedText));
  Init_url = tts_get(Init_encodedText);
  player(Init_url);
}
void loop() {
  audio.loop();
  if (Serial.available()) {  // put streamURL in serial monitor
    audio.stopSong();
    String r = Serial.readString();   // 最多为120个字符
    r.trim();
    if (r.length() > 5) 
    {
      new_encodedText = r;
    }
    String url = tts_get(new_encodedText);
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
