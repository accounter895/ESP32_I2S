#include "Soil_Sensor.h"

float Soil_wet = 0;
uint16_t Soil_int = (AirValue - WaterValue) / 3;

String Soil_judge(void){
  Soil_wet = analogRead(A0);  //put Sensor insert into soil
  String Soil_state = "0";
  if(Soil_wet > WaterValue && Soil_wet < (WaterValue + Soil_int))
  {
    Soil_state = "Very Wet";
  }
  else if(Soil_wet > (WaterValue + Soil_int) && Soil_wet < (AirValue - Soil_int))
  {
    Soil_state = "Wet";
  }
  else if(Soil_wet < AirValue && Soil_wet > (AirValue - Soil_int))
  {
    Soil_state = "Dry";
  }
  return Soil_state;
}