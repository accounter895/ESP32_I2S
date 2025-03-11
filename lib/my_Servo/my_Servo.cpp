#include "my_Servo.h"

Servo My_Servo_A;
Servo My_Servo_B;
Servo My_Servo_C;
Servo My_Servo_D;

void My_Servo_Init(){
    My_Servo_A.setPeriodHertz(50);
    My_Servo_B.setPeriodHertz(50);
    My_Servo_C.setPeriodHertz(50);
    My_Servo_D.setPeriodHertz(50);

    My_Servo_A.attach(PIN_SERVO_A, 500, 2500);
    My_Servo_B.attach(PIN_SERVO_B, 500, 2500);
    My_Servo_C.attach(PIN_SERVO_C, 500, 2500);
    My_Servo_D.attach(PIN_SERVO_D, 500, 2500);

    My_Servo_A.write(90);
    My_Servo_B.write(90);
    My_Servo_C.write(90);
    My_Servo_D.write(90);
}

void My_Servo_A_Move(uint8_t angle){
    if(angle > 180) angle = 180;
    else if(angle < 0) angle = 0;
    My_Servo_A.write(angle);
}

void My_Servo_B_Move(uint8_t angle){
    if(angle > 180) angle = 180;
    else if(angle < 0) angle = 0;
    My_Servo_B.write(angle);
}

void My_Servo_C_Move(uint8_t angle){
    if(angle > 180) angle = 180;
    else if(angle < 0) angle = 0;
    My_Servo_C.write(angle);
}

void My_Servo_D_Move(uint8_t angle){
    if(angle > 180) angle = 180;
    else if(angle < 0) angle = 0;
    My_Servo_D.write(angle);
}

void My_Servo_Move(uint8_t angle_A, uint8_t angle_B, uint8_t angle_C, uint8_t angle_D){
    My_Servo_A_Move(angle_A);
    My_Servo_B_Move(angle_B);
    My_Servo_C_Move(angle_C);
    My_Servo_D_Move(angle_D);
}