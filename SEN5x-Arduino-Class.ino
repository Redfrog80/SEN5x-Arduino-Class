/**
 * @file SEN5x-Arduino-Class.ino
 * @author Redfrog80
 * @brief This is an example program for basic usage 
 * for the SEN5X class.
 * @version 0.2
 * @date 2023-11-28
 * 
 * @copyright Copyright (c) 2023
 * */

#include "Wire.h"

#include "src/SEN_5X.h"

SEN_5X sensor;

void setup(){
    Wire.begin();  
    
    Serial.begin(115200);
    while(!Serial){};

    sensor.start();
}

void loop(){
    if (sensor.isDataReady())
    {
        SEN_5x_Scaled_Measured_Values values = {};
        sensor.getScaledMeasurement(values);
        Serial.print(F("PM: "));
        Serial.print(values.PM1);
        Serial.print(F(", "));
        Serial.print(values.PM25);
        Serial.print(F(", "));
        Serial.print(values.PM4);
        Serial.print(F(", "));
        Serial.print(values.PM10);
        Serial.print(F(", "));
        Serial.print(F("Humidity: "));
        Serial.print(values.CAH);
        Serial.print(F(", Temperature (C): "));
        Serial.print(values.CAT);
        Serial.print(F(", VOC Index: "));
        Serial.println(values.VOC);
    }
    delay(2000);
}
// For debugging purposes
// Prints a buffer (assuming buffer contains checksums)
// in binary format
void printbufwchksum(char* buf, uint8_t size)
{
    for (int row = 0; row < size/3; ++row)
    {
        for (int byte = 0; byte < 3; ++byte)
        {
            if (byte == 2)
                Serial.print(F("   "));
            for (int bit = 7; bit >= 0; bit--)
            {
                Serial.print(bitRead(*(buf+byte+row*3), bit));
            }
        }
        Serial.print(F("\n"));
    }
    Serial.println(F("\n"));
}