/**
 * @file SEN5x-Arduino-Class.ino
 * @author Redfrog80
 * @brief This is an example program for basic usage 
 * for the SEN5X class.
 * @version 0.2.1
 * @date 2024-1-15
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
    sensor.reset();
    sensor.start();
}

void loop(){
    if (sensor.isDataReady())
    {
        SEN_5X_Product_Name params = {};
        sensor.getProductName(params);
        Serial.println(params);

        Serial.print(F("\n"));
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