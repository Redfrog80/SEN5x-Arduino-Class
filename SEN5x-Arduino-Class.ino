/**
 * @file SEN5x-Arduino-Class.ino
 * @author Redfrog80
 * @brief This is an example program for basic usage for (0.1)
 * It will get some information about the sensor, then start
 * printing out measured data in realtime. I'll update this
 * as changes are make to the sensor class.
 * @version 0.1
 * @date 2023-03-19
 * 
 * @copyright Copyright (c) 2023
 * */

#include "Wire.h"

#include "src/SEN_5x.h"

Rf80Sensirion::SEN_5x sensor;
bool ready_flag = false;

uint16_t* unull = nullptr;
int16_t* snull = nullptr;

float mc_pm1p0;
float mc_pm2p5;
float mc_pm4p0; 
float mc_pm10;
float CAH;
float CAT;
float VOC; 
float NOx;

char buf[32] = {};

void setup(){
    Wire.begin();
    Serial.begin(9600);
    while (!Serial){}
    
    Serial.println("Reseting...   ");
    sensor.reset();

    sensor.get_product_name(buf);
    Serial.print("Product name: ");
    Serial.println(buf);

    sensor.get_serial_number(buf);
    Serial.print("Serial number: ");
    Serial.println(buf);

    Serial.println();

    sensor.start_measurement();
    // sensor.start_measurement_RHT_gas_only;
}

void loop(){
    sensor.is_data_ready(&ready_flag);
    if(ready_flag)
    {
        sensor.get_measured_scaled_values(&mc_pm1p0,&mc_pm2p5,&mc_pm4p0,&mc_pm10,&CAH,&CAT,&VOC,&NOx);

        Serial.print(F("PM: "));
        Serial.print(mc_pm1p0);
        Serial.print(F(", "));
        Serial.print(mc_pm2p5);
        Serial.print(F(", "));
        Serial.print(mc_pm4p0);
        Serial.print(F(", "));
        Serial.print(mc_pm10);
        Serial.print(F(", "));
        Serial.print(F("Humidity: "));
        Serial.print(CAH);
        Serial.print(F(", Temperature (C): "));
        Serial.print(CAT);
        Serial.print(F(", VOC Index: "));
        Serial.println(VOC);
    }
}