#include "SEN_5x.h"
void SEN_5X::send_command(uint16_t command)
{
    Wire.beginTransmission(SEN_54_ADDRESS);
    Wire.write((char*)&command,sizeof(command));
    Wire.endTransmission();
    delay(2); 
}