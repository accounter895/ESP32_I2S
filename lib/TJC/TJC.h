#ifndef TJC_H
#define TJC_H

#include <Arduino.h>
#include "my_Sensor.h"

#define TJC Serial1
#define TJC_TX 17
#define TJC_RX 18
#define FRAME_LENGTH 7   // 串口数据帧长度

#define LED_Pin 21

extern DHT_Unified dht;
extern BH1750 lightMeter;

void TJC_Sensor();
void TJC_Light();

#endif
