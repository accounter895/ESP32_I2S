#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

#include <Arduino.h>

#define AirValue    3015        // Air value
#define WaterValue  1180        // Water value
#define Soil_val    A0          // Soil sensor pin

String Soil_judge(void);

#endif