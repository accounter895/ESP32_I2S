#ifndef TJC_H
#define TJC_H

#include <Arduino.h>

#define TJC Serial1
#define TJC_TX 17
#define TJC_RX 18
#define FRAME_LENGTH 7   // 串口数据帧长度

#define LED_Pin 21

void TJC_Sensor(float n0_val, float n1_val, float n4_val, String t5_txt);
void TJC_Light();

#endif
