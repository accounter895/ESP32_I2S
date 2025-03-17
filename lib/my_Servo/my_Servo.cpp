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
    delay(100);
}

void My_Servo_B_Move(uint8_t angle){
    if(angle > 210) angle = 210;
    else if(angle < 0) angle = 0;
    My_Servo_B.write(angle);
    delay(100);
}

void My_Servo_C_Move(uint8_t angle){
    if(angle > 210) angle = 210;
    else if(angle < 0) angle = 0;
    My_Servo_C.write(angle);
    delay(100);
}

void My_Servo_D_Move(uint8_t angle){
    if(angle > 180) angle = 180;
    else if(angle < 0) angle = 0;
    My_Servo_D.write(angle);
    delay(100);
}

void My_Servo_Move(uint8_t angle[4]){
    My_Servo_A_Move(angle[0]);
    My_Servo_B_Move(angle[1]);
    My_Servo_C_Move(angle[2]);
    My_Servo_D_Move(angle[3]);
}

void My_Apple_Move(){
    Model_Pose Apple;
    pinMode(Apple_LED, OUTPUT);
    digitalWrite(Apple_LED, HIGH);
    
    // 定义多个姿态
    uint8_t poses[][4] = {
        {40, 210, 65, 90},
        {90, 210, 150, 90},
        {90, 210, 65, 90},
        {40, 90, 210, 90},
        {90, 210, 65, 90}  // 返回初始姿态
    };

    // 遍历每个姿态并移动机械臂
    for (int i = 0; i < sizeof(poses) / sizeof(poses[0]); i++) {
        memcpy(Apple.Angle, poses[i], sizeof(Apple.Angle));
        My_Servo_Move(Apple.Angle);  // 传递整个数组
        delay(1000);
    }

    digitalWrite(Apple_LED, LOW);
}

void My_Orange_Move(){
    Model_Pose Orange;
    pinMode(Orange_LED, OUTPUT);
    digitalWrite(Orange_LED, HIGH);
    
    // 定义多个姿态
    uint8_t poses[][4] = {
        {40, 210, 65, 90},
        {90, 210, 150, 90},
        {90, 210, 65, 130},
        {40, 90, 210, 130},
        {90, 210, 65, 90}  // 返回初始姿态
    };

    // 遍历每个姿态并移动机械臂
    for (int i = 0; i < sizeof(poses) / sizeof(poses[0]); i++) {
        memcpy(Orange.Angle, poses[i], sizeof(Orange.Angle));
        My_Servo_Move(Orange.Angle);  // 传递整个数组
        delay(1000);
    }

    digitalWrite(Orange_LED, LOW);
}

void My_Banana_Move(){
    Model_Pose Banana;
    pinMode(Banana_LED, OUTPUT);
    digitalWrite(Banana_LED, HIGH);
    
    // 定义多个姿态
    uint8_t poses[][4] = {
        {40, 210, 65, 90},
        {90, 210, 150, 90},
        {90, 210, 65, 50},
        {40, 90, 210, 50},
        {90, 210, 65, 90}  // 返回初始姿态
    };

    // 遍历每个姿态并移动机械臂
    for (int i = 0; i < sizeof(poses) / sizeof(poses[0]); i++) {
        memcpy(Banana.Angle, poses[i], sizeof(Banana.Angle));
        My_Servo_Move(Banana.Angle);  // 传递整个数组
        delay(1000);
    }

    digitalWrite(Banana_LED, LOW);
}
