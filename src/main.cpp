#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include "Arduino.h"
#include "WiFiMulti.h"
#include "Audio.h"

// 1、修改MAX98357喇叭接口
#define I2S_DOUT    13
#define I2S_BCLK    2
#define I2S_LRC     15

Audio audio;
WiFiMulti wifiMulti;
// 2、修改WiFi密码
const char *ssid = "Redmi Note 12 Turbo";
const char *password = "12345678";  // Change this to your WiFi password
// 3、修改百度语音助手的用户信息
const char *API_KEY = "UjvYp1JLIO8yEqAutOVJT5WR";
const char *SECRET_KEY = "anMdGCLferXzIIeAUS8HtImqbCWpUQAw";
// 4、修改播放文本内容
String encodedText = "你好，我是量子位";

const int PER = 4;
const int SPD = 5;
const int PIT = 5;
const int VOL = 5;
const int AUE = 6;

const char *TTS_URL = "https://tsn.baidu.com/text2audio";
String url = TTS_URL;

void player();
void tts_get();

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  encodedText = urlEncode(urlEncode(encodedText));
  tts_get();
  player();
}
void tts_get() {
  const char *headerKeys[] = { "Content-Type", "Content-Length" };
  // 5、修改百度语音助手的token
  url += "?tok=24.dcb0788463590edacd07841f35d2bb5f.2592000.1743050163.282335-117723335";
  url += "&tex=" + encodedText;
  url += "&per=" + String(PER);
  url += "&spd=" + String(SPD);
  url += "&pit=" + String(PIT);
  url += "&vol=" + String(VOL);
  url += "&aue=" + String(AUE);
  url += "&cuid=esp32s3";
  url += "&lan=zh";
  url += "&ctp=1";

  HTTPClient http;

  Serial.print("URL: ");
  Serial.println(url);

  http.begin(url);
  http.collectHeaders(headerKeys, 2);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    if (httpResponseCode == HTTP_CODE_OK) {
      Serial.print("Content-Type = ");
      Serial.println(http.header("Content-Type"));
      String contentType = http.header("Content-Type");
      if (contentType.startsWith("audio")) {
        Serial.println("合成成功，返回的是音频文件");
        // 处理音频文件，保存到SD卡或者播放
      } else if (contentType.equals("application/json")) {
        Serial.println("合成出现错误,返回的是JSON文本");
        // 处理错误信息，根据需要进行相应的处理
      } else {
        Serial.println("未知的Content-Type");
        // 可以添加相应的处理逻辑
      }
    } else {
      Serial.println("Failed to receive audio file");
    }
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
void loop() {
  audio.loop();
  if (Serial.available()) {  // put streamURL in serial monitor
    audio.stopSong();
    String r = Serial.readString();
    r.trim();
    if (r.length() > 5) audio.connecttohost(r.c_str());
    log_i("free heap=%i", ESP.getFreeHeap());
  }
}

void player() {
  const char *host = url.c_str();
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(12);        // 0...21
  audio.connecttohost(host);  //  128k mp3
}



