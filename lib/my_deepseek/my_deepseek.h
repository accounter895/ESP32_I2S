#ifndef MY_DEEPSEEK_H
#define MY_DEEPSEEK_H

#include <stdio.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class My_GPT{
public:
    String getGPTAnswer(String inputText);
};

#endif