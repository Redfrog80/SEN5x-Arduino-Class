/**
 * @file SEN_5X.h
 * @author Redfrog80
 * @brief This is the header file for the SEN5X class
 * @version 0.2
 * @date 2023-11-28
 * 
 * @copyright Copyright (c) 2023
 * */

#ifndef SEN_5X_HEADER
#define SEN_5X_HEADER

#include "SEN_5X_TYPES.h"
#include "SEN_5x_COMMANDS.h"
#include "arduino.h"
#include "Wire.h"

#define SEN_54_ADDRESS uint8_t(0x69)

class SEN_5X
{
    uint8_t buf[48] = {};

    inline void send_command(uint16_t command);
    
    uint8_t generate_checksum(uint8_t buf[2]);
    
    void generate_checksum(uint8_t* buf, uint8_t size);

    SEN_5X_STATUS_CODES validate_checksum(uint8_t* buf, uint8_t size);

    inline void swap_endianess_words(uint8_t* buf, uint8_t size);
    inline void swap_endianess_word(uint16_t& n);
    inline void swap_endianess(uint8_t* buf, uint8_t size);
    
    inline void truncate_checksum(uint8_t* buf, uint8_t size);

    SEN_5X_STATUS_CODES collectData(const uint16_t command, const uint8_t& size, const uint8_t d = 20);
    SEN_5X_STATUS_CODES collectData(const uint16_t command, const uint8_t& size, void* dest, const uint8_t d = 20);

    void read(uint8_t* buf, uint8_t size);
    void write(uint8_t* buf, uint8_t size);

    public:
    
    void start();
    void startRHTGasOnly();
    void stop();
    void reset();

    bool isDataReady();

    SEN_5X_STATUS_CODES getUnscaledMeasurement(SEN_5X_Measured_Values& values);
    SEN_5X_STATUS_CODES getScaledMeasurement(SEN_5x_Scaled_Measured_Values& values);

    SEN_5X_STATUS_CODES getTempCompensationParams(SEN_5X_Temperature_Compensation_Params& params);
    SEN_5X_STATUS_CODES setTempCompensationParams(SEN_5X_Temperature_Compensation_Params& params);

    SEN_5X_STATUS_CODES getWarmStartParam(SEN_5X_Warm_Start_Param& param);
    SEN_5X_STATUS_CODES setWarmStartParam(SEN_5X_Warm_Start_Param& param);

    SEN_5X_STATUS_CODES getVOCTuningParams(SEN_5X_VOC_Algorithm_Tuning_Params& params);
    SEN_5X_STATUS_CODES setVOCTuningParams(SEN_5X_VOC_Algorithm_Tuning_Params& params);

    SEN_5X_STATUS_CODES getNOxTuningParams(SEN_5X_NOX_Algorithm_Tuning_Params& params);
    SEN_5X_STATUS_CODES setNOxTuningParams(SEN_5X_NOX_Algorithm_Tuning_Params& params);

    SEN_5X_STATUS_CODES getRHTAccelMode(SEN_5X_RHT_Accel_Mode& mode);
    SEN_5X_STATUS_CODES setRHTAccelMode(SEN_5X_RHT_Accel_Mode& mode);

    SEN_5X_STATUS_CODES getVOCAlgorithmState(SEN_5X_VOC_Algorithm_State& state);
    SEN_5X_STATUS_CODES setVOCAlgorithmState(SEN_5X_VOC_Algorithm_State& state);

    void startFanCleaning();

    SEN_5X_STATUS_CODES getFanCleaningInterval(SEN_5X_Auto_Cleaning_Interval& interval);
    SEN_5X_STATUS_CODES setFanCleaningInterval(SEN_5X_Auto_Cleaning_Interval& interval);

    SEN_5X_STATUS_CODES getProductName(SEN_5X_Product_Name& name);
    SEN_5X_STATUS_CODES getSerialNumber(SEN_5X_Serial_Number& serialNumber);
    SEN_5X_STATUS_CODES getFirmwareVersion(SEN_5X_Firmware_Version& firmwareVersion);
    
    SEN_5X_STATUS_CODES getDeviceStatus(SEN_5X_Device_Status& status);
    
    bool isFanSpeedOk();
    bool isFanCleaning();
    bool isGasSensorRunning();
    bool isRHTCommOk();
    bool isLaserOk();
    bool isFanOk();
    
    void clearDeviceStatus();
};

#endif //SEN_5X_HEADER
