#include "my_wifi.h"

const char* ssid = WIFI_SSID;               // 若用手机热点，需要调整为2.4G频段
const char* password = WIFI_PASSWORD;

void wifi_setup() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.print("WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());
}