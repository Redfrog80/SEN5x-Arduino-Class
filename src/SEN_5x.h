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
    
    bool validate_checksum(uint8_t* buf, uint8_t size);

    void generate_checksum(uint8_t* buf, uint8_t size);

    inline void swap_endianess(uint8_t* buf, uint8_t size);
    inline void swap_endianess_uint16(uint16_t& n);
    
    inline void truncate_checksum(uint8_t* buf, uint8_t size);

    void collectData(const uint16_t command, const uint8_t& size, const uint8_t d = 20);
    void collectData(const uint16_t command, const uint8_t& size, void* dest, const uint8_t d = 20);

    void read(uint8_t* buf, uint8_t size);
    void write(uint8_t* buf, uint8_t size);

    public:
    
    void start();
    void startRHTGasOnly();
    void stop();
    void reset();

    bool isDataReady();

    void getUnscaledMeasurement(SEN_5X_Measured_Values& values);
    void getScaledMeasurement(SEN_5x_Scaled_Measured_Values& values);

    void getTempCompensationParams(SEN_5X_Temperature_Compensation_Params& params);
    void setTempCompensationParams(SEN_5X_Temperature_Compensation_Params& params);

    void getWarmStartParam(SEN_5X_Warm_Start_Param& param);
    void setWarmStartParam(SEN_5X_Warm_Start_Param& param);

    void getVOCTuningParams(SEN_5X_VOC_Algorithm_Tuning_Params& params);
    void setVOCTuningParams(SEN_5X_VOC_Algorithm_Tuning_Params& params);

    void getNOxTuningParams(SEN_5X_NOX_Algorithm_Tuning_Params& params);
    void setNOxTuningParams(SEN_5X_NOX_Algorithm_Tuning_Params& params);

    void getRHTAccelMode(SEN_5X_RHT_Accel_Mode& mode);
    void setRHTAccelMode(SEN_5X_RHT_Accel_Mode& mode);

    void getVOCAlgorithmState(SEN_5X_VOC_Algorithm_State& state);
    void setVOCAlgorithmState(SEN_5X_VOC_Algorithm_State& state);

    void startFanCleaning();

    void getFanCleaningInterval(SEN_5X_Auto_Cleaning_Interval& interval);
    void setFanCleaningInterval(SEN_5X_Auto_Cleaning_Interval& interval);

    void getProductName(SEN_5X_Product_Name& name);
    void getSerialNumber(SEN_5X_Serial_Number& serialNumber);
    void getFirmwareVersion(SEN_5X_Firmware_Version& firmwareVersion);
    
    void getDeviceStatus(SEN_5X_Device_Status& status);
    
    bool isFanSpeedOk();
    bool isFanCleaning();
    bool isGasSensorRunning();
    bool isRHTCommOk();
    bool isLaserOk();
    bool isFanOk();
    
    void clearDeviceStatus();
};

#endif //SEN_5X_HEADER
