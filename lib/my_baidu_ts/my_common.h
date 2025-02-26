#ifndef MY_COMMON
#define MY_COMMON
#if 1
#include <Arduino.h>
#include "base64.h"
#include <WiFiClientSecure.h>
#include "HTTPClient.h"
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>
#include <Wire.h>
#include <SD.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// 与AP模式和Web服务器有关的库
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#endif
 
#define ESP_SUCCEED 0
#define ESP_FAILT 1
 
#define BIT(n)             (1LLU << (n))
#define SET_BIT(mask, n)   ((mask) |= BIT(n))
#define RESET_BIT(mask, n) ((mask) &= ~BIT(n))
#define ISSET_BIT(mask, n) (!!((mask)&BIT(n)))
#define LLM_STANDBY_MODE              BIT(1) //待机状态标记
#define LLM_CONTINUOUS_DIALOGUE_STATE BIT(2) //连续对话状态标记
#define LLM_TTS_STANDBY_STATE         BIT(3) //TTS线程进入待机状态
#define LLM_RECORDING_STANDBY_STATE   BIT(4) //音频数据采集线程进入待机状态
#define LLM_REAWAKEN_FLAG             BIT(5) //重复唤醒标记
#define LLM_TTS_POLL_FLAG             BIT(6) //POLL task结束标记
#define LLM_PLAY_AUDIO_FLAG           BIT(7) //音频播放状态,置位1--播放中
#define LLM_FIRST_RSP_FLAG            BIT(8) //大模型响应标记,第一个响应后置位，即哪个模型响应快使用哪个做TTS
 
int g_current_state = 0;
 
typedef struct LLM_MSG_RSP
{
    String err_msg;
    int err_no;
}LLM_MSG_RSP_T;
 
#endif