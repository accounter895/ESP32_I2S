#ifndef MY_BAIDU_TS_H
#define MY_BAIDU_TS_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <base64.h>
#include <cJSON.h>
#include <UrlEncode.h>
#include "my_common.h"
#include "my_i2s.h"
#include "Audio.h"

String stt_tts_setup();
String sendToSTT(uint16_t *data);
String sendToTTS(String InputText, int *len);
void audio_play_by_text(String input);
void tts_get();

#endif
