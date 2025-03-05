#ifndef MY_SENSOR_H
#define MY_SENSOR_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <MQ135.h>
#include <BH1750.h>
#include <Wire.h>
#include "Soil_Sensor.h"

#define DHTTYPE    DHT11     // DHT 22 (AM2302)
#define DHTPIN     2
#define PIN_MQ135  A2
#define Soil_val   A0
#define SDA 38
#define SCL 39
#endif