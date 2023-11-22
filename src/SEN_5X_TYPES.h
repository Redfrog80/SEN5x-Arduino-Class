#ifndef SEN_5X_TYPES
#define SEN_5X_TYPES

const enum SEN_5X_MODES
{
    IDLE_MODE,
    FULL_MEASUREMENT_MODE,
    RHT_GAS_ONLY_MODE
};

typedef union 
{
    uint8_t Raw[3];
    struct
    {
        uint8_t Data[2];
        uint8_t Data_CS;
    };
} SEN_5X_Raw_Packet;

typedef union
{
    uint8_t Raw[24];
    SEN_5X_Raw_Packet Packets[8];
    struct 
    {
        uint16_t PM1;
        uint8_t PM1_CS;
        uint16_t PM25;
        uint8_t PM25_CS;
        uint16_t PM5;
        uint8_t PM5_CS;
        uint16_t PM10;
        uint8_t PM10_CS;
        int16_t CAH;
        uint8_t CAH_CS;
        int16_t CAT;
        uint8_t CAT_CS;
        int16_t VOC;
        uint8_t VOC_CS;
        int16_t NOX;
        uint8_t NOX_CS;
    };
} SEN_5X_Measured_Values;

typedef struct
{
        float PM1;
        float PM25;
        float PM5;
        float PM10;
        float CAH;
        float CAT;
        float VOC;
        float NOX;
} SEN_5x_Scaled_Measured_Values;

typedef union
{
    uint8_t Raw[9];
    SEN_5X_Raw_Packet Packets[3];
    struct
    {
        int16_t TempOffset;
        uint8_t TempOffset_CS;
        int16_t NormTempOffsetM;
        uint8_t NormTempOffsetM_CS;
        int16_t TimeConstant;
        uint8_t TimeConstant_CS;
    };
} SEN_5X_Temperature_Compensation_Params;

typedef union
{
    uint8_t Raw[3];
    SEN_5X_Raw_Packet Packets[1];
    struct
    {
        uint16_t WarmStart;
        uint8_t WarmStart_CS;
    };
} SEN_5X_Warm_Start_Params;

typedef union
{
    uint8_t Raw[18];
    SEN_5X_Raw_Packet Packets[6];
    struct 
    {
        int16_t IndexOffset;
        uint8_t IndexOffset_CS;
        int16_t LearningTimeOffsetHrs;
        uint8_t LearningTimeOffsetHrs_CS;
        int16_t LearningTimeGainHrs;
        uint8_t LearningTimeGainHrs_CS;
        int16_t GatingMaxDurationMins;
        uint8_t GatingMaxDurationMins_CS;
        int16_t StdInitial;
        uint8_t StdInitial_CS;
        int16_t GainFactor;
        uint8_t GainFactor_CS;
    };
    
} SEN_5X_VOC_Algorithm_Tuning_Params;

typedef union
{
    uint8_t Raw[18];
    SEN_5X_Raw_Packet Packets[6];
    struct 
    {
        int16_t IndexOffset;
        uint8_t IndexOffset_CS;
        int16_t LearningTimeOffsetHrs;
        uint8_t LearningTimeOffsetHrs_CS;
        int16_t LearningTimeGainHrs;
        uint8_t LearningTimeGainHrs_CS;
        int16_t GatingMaxDurationMins;
        uint8_t GatingMaxDurationMins_CS;
        int16_t StdInitial;
        uint8_t StdInitial_CS;
        int16_t GainFactor;
        uint8_t GainFactor_CS;
    };
    
} SEN_5X_NOX_Algorithm_Tuning_Params;

typedef union
{
    uint8_t Raw[3];
    SEN_5X_Raw_Packet Packets[1];
    struct
    {
        uint16_t AccelMode;
        uint8_t AccelMode_CS;
    };
} SEN_5X_RHT_Accel_Mode;

typedef union
{
    uint8_t Raw[12];
    SEN_5X_Raw_Packet Packets[4];
} SEN_5X_VOC_Algorithm_State;

typedef union
{
    uint8_t Raw[6];
    SEN_5X_Raw_Packet Packets[2];
} SEN_5X_Auto_Cleaning_Interval;

typedef union
{
    uint8_t Raw[48];
    SEN_5X_Raw_Packet Packets[16];
} SEN_5X_Product_Name;

typedef union
{
    uint8_t Raw[48];
    SEN_5X_Raw_Packet Packets[16];
} SEN_5X_Serial_Number;

typedef union 
{
    uint8_t Raw[3];
    SEN_5X_Raw_Packet Packets[1];
    struct
    {
        uint8_t FirmwareVersion;
        uint8_t Reserved;
        uint8_t CS;
    };
} SEN_5X_Firmware_Version;

typedef union 
{
    uint8_t Raw[6];
    SEN_5X_Raw_Packet Packets[2];
} SEN_5X_Device_Status;

#endif //SEN_5X_TYPES