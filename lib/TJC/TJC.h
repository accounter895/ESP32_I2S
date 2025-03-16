#ifndef TJC_H
#define TJC_H

#include <Arduino.h>
#include "my_Sensor.h"
#include "my_Servo.h"

#define TJC Serial1
#define TJC_TX 17
#define TJC_RX 18
#define FRAME_LENGTH 9   // 串口数据帧长度
#define Servo_Step 25
#define Init_Angle 90

#define LED_Pin 21

extern DHT_Unified dht;
extern BH1750 lightMeter;

void TJC_Sensor();
void TJC_Light();

#endif
