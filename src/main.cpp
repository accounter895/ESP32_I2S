// Welcome to www.LingShunLAB.com

#include "Arduino.h"
#include "WiFiMulti.h"
#include "Audio.h"

// Digital I/O used
#define I2S_DOUT      13
#define I2S_BCLK      2
#define I2S_LRC       15

Audio audio;
WiFiMulti wifiMulti;

// config your wifi
String ssid =     "Redmi Note 12 Turbo";
String password = "12345678";


void setup() {

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(ssid.c_str(), password.c_str());
    wifiMulti.run();
    if(WiFi.status() != WL_CONNECTED){
        WiFi.disconnect(true);
        wifiMulti.run();
    }

    // 设置较大的缓冲区大小（例如：16KB RAM, 64KB PSRAM）
    //audio.setBufsize(131072, 1048576);  // 根据需要调整大小

    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(12); // 0...21

    audio.connecttohost("https://samplelib.com/lib/preview/mp3/sample-6s.mp3"); //  128k mp3
}

void loop()
{
    audio.loop();
    if(Serial.available()){ // put streamURL in serial monitor
        audio.stopSong();
        String r=Serial.readString(); r.trim();
        if(r.length()>5) audio.connecttohost(r.c_str());
    }
    log_i("free heap=%i", ESP.getFreeHeap());
}

// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}