#ifndef MY_SERVO_H
#define MY_SERVO_H

#include <Arduino.h>
#include <ESP32Servo.h>

#define PIN_SERVO_A 4
#define PIN_SERVO_B 5
#define PIN_SERVO_C 6
#define PIN_SERVO_D 7

#define SA_Init_Angle 90
#define SB_Init_Angle 90
#define SC_Init_Angle 90
#define SD_Init_Angle 90

#define Apple_LED   19
#define Banana_LED  20
#define Orange_LED  21

typedef struct
{
    uint8_t Angle_A = 40;      // 角度增加，夹子收紧；角度减小，夹子张开（40度为张开限幅，90为闭合限幅）
    uint8_t Angle_B = 180;     // 角度增加，小臂下压；角度减小，小臂上抬
    uint8_t Angle_C = 65;      // 角度增加，大臂下压；角度减小，大臂上抬
    uint8_t Angle_D = 90;      // 角度增加，底座往左转；角度减小，底座往右转
    uint8_t Angle[4] = {40, 180, 65, 90};
    uint8_t Servo_poses[][4];
} Model_Pose;

void My_Servo_Init();
void My_Servo_Move(uint8_t angle_A[4]);
void My_Apple_Move();
void My_Banana_Move();
void My_Orange_Move();

#endif
