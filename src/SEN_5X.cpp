/**
 * @file SEN_5X.cpp
 * @author Redfrog80
 * @brief This is the implementation file for the SEN5X class
 * @version 0.2.1
 * @date 2024-1-15
 * 
 * @copyright Copyright (c) 2023
 * */
#include "SEN_5x.h"
inline void SEN_5X::send_command(uint16_t command)
{
    swap_endianess_word(command);
    write((uint8_t*)&command, sizeof(command));
}

inline void SEN_5X::swap_endianess_words(uint8_t* buf, uint8_t size)
{
    for (uint8_t i = 0; i < size; i+=2)
        swap_endianess_word(*(uint16_t*)(buf + i));
}

inline void SEN_5X::swap_endianess_word(uint16_t& n)
{
    n = (n >> 8) | (n << 8);
}

inline void SEN_5X::swap_endianess(uint8_t* buf, uint8_t size)
{
    for (uint8_t i = 0; i < size; ++i)
    {
        uint8_t byte = buf[i];
        buf[i] = buf[size-i-1];
        buf[size-i-1] = byte;
    }
}

inline void SEN_5X::truncate_checksum(uint8_t* buf, uint8_t size)
{
    for(uint8_t i = 0; i < size/3; ++i)
        memmove((buf + i * 2),(buf + i * 3),sizeof(uint16_t));
}

SEN_5X_STATUS_CODES SEN_5X::collectData(const uint16_t command, const uint8_t& size, const uint8_t d)
{
    send_command(command);
    delay(d);
    read((uint8_t*)&buf, size);
    
    return validate_checksum(buf, size);
    //TODO Go through and specify endianess stuff per method instead of trying to encapsulate it
    // if (error != SEN_5X_STATUS_OK) return error;

    // truncate_checksum(buf,size);

    // swap_endianess_words(buf, size-size/3);
    
}

void SEN_5X::write(uint8_t* buf, uint8_t size)
{
    Wire.beginTransmission(SEN_54_ADDRESS);
    while (size--) Wire.write(*buf++);
    Wire.endTransmission();
    delay(2);
}

void SEN_5X::read(uint8_t* buf, uint8_t size)
{
    Wire.requestFrom(SEN_54_ADDRESS,size);
    while (size--) *buf++ = Wire.read();
}

void SEN_5X::start()
{
    send_command(SEN_5X_START_MEASUREMENT);
    delay(50);
};
void SEN_5X::startRHTGasOnly()
{
    send_command(SEN_5X_START_MEASUREMENT_RHT_GAS_ONLY);
    delay(50);
};
void SEN_5X::stop()
{
    send_command(SEN_5X_STOP_MEASUREMENT);
    delay(200);
}
void SEN_5X::reset()
{
    send_command(SEN_5X_RESET);
    delay(100);
}

uint8_t SEN_5X::generate_checksum(uint8_t buf[2])
{
    uint8_t crc = 0xFF;
    for(int i = 0; i < 2; i++) 
    {
        crc ^= buf[i];
        for(uint8_t bit = 8; bit > 0; --bit) 
        {
            if(crc & 0x80) 
                crc = (crc << 1) ^ 0x31u;
            else 
                crc = (crc << 1);
        }
    }
    return crc;
}

void SEN_5X::generate_checksum(uint8_t* buf, uint8_t size)
{
    for (uint8_t j = 0; j < size/3; ++j)
    {
        uint8_t offset = j*3;
        uint8_t crc = generate_checksum(buf+offset);
        buf[offset+2] = crc;
    }
}

SEN_5X_STATUS_CODES SEN_5X::validate_checksum(uint8_t* buf, uint8_t size)
{
    for (uint8_t j = 0; j < size/3; ++j)
    {
        uint8_t offset = j*3;
        uint8_t crc = generate_checksum(buf+offset);
        if (buf[offset+2] != crc)
            return SEN_5X_STATUS_ERROR_BAD_CHECKSUM;
    }
    return SEN_5X_STATUS_OK;
}

bool SEN_5X::isDataReady()
{
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_DATA_READY_FLAG,3);
    if (error != SEN_5X_STATUS_OK) return false;

    return buf[1];
}
SEN_5X_STATUS_CODES SEN_5X::getUnscaledMeasurement(SEN_5X_Measured_Values& values)
{
    const uint8_t size = 24;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_MEASURED_VALUES, size);
    if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    swap_endianess_words(buf, size-size/3);
    
    memcpy(&values, buf, size-size/3);

    return error;
}
SEN_5X_STATUS_CODES SEN_5X::getScaledMeasurement(SEN_5x_Scaled_Measured_Values& values)
{
    const uint8_t size = 24;
    const SEN_5X_Measured_Values* usvalues = (const SEN_5X_Measured_Values*)buf;
    
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_MEASURED_VALUES, size);
    if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    swap_endianess_words(buf, size-size/3);

    values.PM1 = usvalues->PM1/10.f;
    values.PM25 = usvalues->PM25/10.f;
    values.PM4 = usvalues->PM4/10.f;
    values.PM10 = usvalues->PM10/10.f;
    values.CAH = usvalues->CAH/100.f;
    values.CAT = usvalues->CAT/200.f;
    values.VOC = usvalues->VOC/10.f;
    values.NOX = usvalues->NOX/10.f;

    return error;
}

SEN_5X_STATUS_CODES SEN_5X::getTempCompensationParams(SEN_5X_Temperature_Compensation_Params& params)
{
    const uint8_t size = 9;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_WRITE_TEMP_COMP_PARAMETERS, size);
    if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    swap_endianess_words(buf, size-size/3);
    memcpy(&params, buf, size-size/3);
    return error;
}

SEN_5X_STATUS_CODES SEN_5X::getWarmStartParam(SEN_5X_Warm_Start_Param& params)
{
    const uint8_t size = 9;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_WRITE_WARM_START_PARAMETERS, size);
    if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    swap_endianess_words(buf, size-size/3);
    memcpy(&params, buf, size-size/3);
    return error;
}

SEN_5X_STATUS_CODES SEN_5X::getVOCTuningParams(SEN_5X_VOC_Algorithm_Tuning_Params& params)
{
    const uint8_t size = 18;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_WRITE_VOC_ALG_TUNING_PARAMETERS, size);
    if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    swap_endianess_words(buf, size-size/3);
    memcpy(&params, buf, size-size/3);
    return error;
}

SEN_5X_STATUS_CODES SEN_5X::getNOxTuningParams(SEN_5X_NOX_Algorithm_Tuning_Params& params)
{
    const uint8_t size = 18;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_WRITE_NOX_ALG_TUNING_PARAMETERS, size);
    if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    swap_endianess_words(buf, size-size/3);
    memcpy(&params, buf, size-size/3);
    return error;
}

SEN_5X_STATUS_CODES SEN_5X::getRHTAccelMode(SEN_5X_RHT_Accel_Mode& mode)
{
    const uint8_t size = 3;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_WRITE_RHT_ACCELERATION_MODE, size);
    if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    swap_endianess_words(buf, size-size/3);
    memcpy(&mode, buf, size-size/3);
    return error;
}

SEN_5X_STATUS_CODES SEN_5X::getVOCAlgorithmState(SEN_5X_VOC_Algorithm_State& state)
{
    const uint8_t size = 12;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_WRITE_VOC_ALG_STATE, size);
    if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    swap_endianess(buf, size-size/3);
    memcpy(&state, buf, size-size/3);
    return error;
}

void SEN_5X::startFanCleaning()
{
    send_command(SEN_5X_START_FAN_CLEANING);
    delay(20);
}

SEN_5X_STATUS_CODES SEN_5X::getFanCleaningInterval(SEN_5X_Auto_Cleaning_Interval& interval)
{
    const uint8_t size = 6;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_WRITE_AUTO_CLEANING_INITERVAL, size);
    if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    swap_endianess_words(buf, size-size/3);
    memcpy(&interval, buf, size-size/3);
    return error;
}

SEN_5X_STATUS_CODES SEN_5X::getProductName(SEN_5X_Product_Name& name)
{
    const uint8_t size = 48;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_PRODUCT_NAME, size);
    // Apparently the sensor does not provide valid checksums past the null char
    // of the string, meaning the reamaining data you should be able to read will
    // not have a valid checksum. For the sake of simplicity, I've commented the 
    // following line out

    // if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    memcpy(&name, buf, size-size/3);
    return SEN_5X_STATUS_OK;
}
SEN_5X_STATUS_CODES SEN_5X::getSerialNumber(SEN_5X_Serial_Number& serialNumber)
{ 
    const uint8_t size = 48;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_SERIAL_NUMBER, size);
    
    // Apparently the sensor does not provide valid checksums past the null char
    // of the string, meaning the reamaining data you should be able to read will
    // not have a valid checksum. For the sake of simplicity, I've commented the 
    // following line out

    // if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    memcpy(&serialNumber, buf, size-size/3);
    return SEN_5X_STATUS_OK;
}
SEN_5X_STATUS_CODES SEN_5X::getFirmwareVersion(SEN_5X_Firmware_Version& firmwareVersion)
{
    const uint8_t size = 3;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_FIRMWARE_VERSION, size);
    if (error != SEN_5X_STATUS_OK) return error;

    firmwareVersion = buf[0];

    return error;
}
SEN_5X_STATUS_CODES SEN_5X::getDeviceStatus(SEN_5X_Device_Status& status)
{
    const uint8_t size = 6;
    SEN_5X_STATUS_CODES error = collectData(SEN_5X_READ_DEVICE_STATUS, size);
    if (error != SEN_5X_STATUS_OK) return error;

    truncate_checksum(buf,size);

    swap_endianess(buf, size-size/3);
    memcpy(&status, buf, size-size/3);
    return error;
}

// bool SEN_5X::isFanSpeedOk()
// {
//     collectData(SEN_5X_READ_DEVICE_STATUS,6);

//     return *(uint32_t*)buf & 0x00000200;
// }
// bool SEN_5X::isFanCleaning()
// {
//     collectData(SEN_5X_READ_DEVICE_STATUS,6);

//     return *(uint32_t*)buf & 0x00008000;
// }
// bool SEN_5X::isGasSensorRunning()
// {
//     collectData(SEN_5X_READ_DEVICE_STATUS,6);

//     return *(uint32_t*)buf & 0x00000000;
// }
// bool SEN_5X::isRHTCommOk()
// {

// }
// bool SEN_5X::isLaserOk()
// {

// }
// bool SEN_5X::isFanOk()
// {

// }