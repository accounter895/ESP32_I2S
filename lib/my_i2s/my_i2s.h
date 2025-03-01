#ifndef MY_I2S_H
#define MY_I2S_H

#include <Arduino.h>
#include <hal/i2s_types.h>
#include "driver/i2s.h"

#endif

#define SAMPLE_RATE (16000)         // 采样率为16Khz
#define bufferlen 128                // 缓冲区长度

// 修改INMP441麦克风接口
#define INMP441_WS       15         
#define INMP441_BCK      2          
#define INMP441_SD       13         

// 修改MAX98357喇叭接口
#define MAX98357_DIN     9
#define MAX98357_BCLK    46
#define MAX98357_LRC     8

const i2s_port_t I2S_PORT_INMP441 = I2S_NUM_0;        // I2S外设INMP441端口号为0
const i2s_port_t I2S_PORT_MAX98357A = I2S_NUM_1;      // I2S外设MAX98357A端口号为1

class My_I2S {
public:
    esp_err_t err;                          // 错误return
    void my_inmp441_init();                 // 麦克风I2S初始化
    void my_max98357a_init();               // 扬声器I2S初始化
    void inmp441_max98357_loop();           // 读取麦克风数据并发送到扬声器
};

