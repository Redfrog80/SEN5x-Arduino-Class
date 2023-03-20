/**
 * @file SEN_5x.h
 * @author Redfrog80
 * @brief This is very WIP class for the SEN5x series of
 * environment sensors. I cannot gurantee forward/backwards
 * compatibility of methods until 1.0.
 * @version 0.1
 * @date 2023-03-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SEN_5X_HEADER
#define SEN_5X_HEADER

#include "arduino.h"
#include "Wire.h"

namespace Rf80Sensirion{

    #include "SEN_5x_REG_COMMANDS.h"

    #define INVALID_READ uint16_t(0xFFFF)
    #define SEN_5x_IDLE_MODE uint8_t(0x00)
    #define SEN_5x_MEASUREMENT_MODE uint8_t(0x01)
    #define SEN_5x_MEASUREMENT_MODE_RHT_GAS_ONLY uint8_t(0x02)

    #define DATA_STATE_OK 0
    #define DATA_STATE_IS_NULLPOINTER 1
    #define DATA_STATE_FAILED_CHECKSUM 2

    #define SEN_54_ADDRESS uint8_t(0x69)

    class SEN_5x
    {
        private:
            uint8_t __SEN_STATE__ = SEN_5x_IDLE_MODE;
            
            uint8_t calculate_crc(uint8_t* buf, uint8_t offset) 
            {
                uint8_t crc = 0xFF;
                for(uint8_t i = 0; i < 2; i++) 
                {
                    crc ^= buf[i + offset];

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
            void send_data(uint16_t data)
            {
                uint8_t i_buf[2] = {(data >> 8), data};

                Wire.beginTransmission(SEN_54_ADDRESS);
                Wire.write(i_buf[0]);
                Wire.write(i_buf[1]);
                Wire.write(calculate_crc(i_buf,0));
                Wire.endTransmission();
                delay(2); 
            }
            void send_command(uint16_t data)
            {
                uint8_t i_buf[2] = {(data >> 8), data};

                Wire.beginTransmission(SEN_54_ADDRESS);
                Wire.write(i_buf[0]);
                Wire.write(i_buf[1]);
                Wire.endTransmission();
                delay(2); 
            }
            
            template <class T>
            uint8_t read_data(T* data)
            {  
                uint8_t i_buf[3] = {};

                for (uint8_t byte_num = 0; byte_num < 3; ++byte_num)
                    i_buf[byte_num] = Wire.read();

                if (data)
                    if (calculate_crc(i_buf,0) == i_buf[2])
                    {
                        *data = (T)(i_buf[0] << 8 | i_buf[1]);
                        return DATA_STATE_OK;
                    }
                    else
                        return DATA_STATE_FAILED_CHECKSUM;
                else
                    return DATA_STATE_IS_NULLPOINTER;

            }

        public:
            SEN_5x(){};

            void start_measurement()
            {
                send_command(START_MEASUREMENT);
                delay(50);

                __SEN_STATE__ = SEN_5x_MEASUREMENT_MODE;
            }

            void start_measurement_RHT_gas_only()
            {
                send_command(START_MEASUREMENT_RHT_GAS_ONLY);
                delay(50);

                __SEN_STATE__ = SEN_5x_MEASUREMENT_MODE_RHT_GAS_ONLY;
            }

            void stop_measurement()
            {
                send_command(STOP_MEASUREMENT);
                delay(200);

                __SEN_STATE__ = SEN_5x_IDLE_MODE;;
            }

            bool is_data_ready(bool* flag)
            {
                send_command(READ_DATA_READY_FLAG);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(3));

                return !read_data(flag);
            }
            
            template <class Unsigned_type, class Signed_type>
            bool get_measured_scaled_values(Unsigned_type* mc_pm1p0, Unsigned_type* mc_pm2p5,
                                        Unsigned_type* mc_pm4p0, Unsigned_type* mc_pm10, 
                                        Signed_type* CAH, Signed_type* CAT, Signed_type* VOC,
                                        Signed_type* NOx)
            {
                if (__SEN_STATE__ == SEN_5x_IDLE_MODE)
                    return false;
                
                const uint8_t num_values = 8;

                send_command(READ_MEASURED_VALUES);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*3));

                uint8_t error;

                error = read_data(mc_pm1p0);
                if (error==DATA_STATE_OK) *mc_pm1p0 /= 10; 

                error = read_data(mc_pm2p5);
                if (error==DATA_STATE_OK) *mc_pm2p5 /= 10; 

                error = read_data(mc_pm4p0);
                if (error==DATA_STATE_OK) *mc_pm4p0 /= 10; 

                error = read_data(mc_pm10);
                if (error==DATA_STATE_OK) *mc_pm10 /= 10; 

                error = read_data(CAH);
                if (error==DATA_STATE_OK) *CAH /= 100; 

                error = read_data(CAT);
                if (error==DATA_STATE_OK) *CAT /= 200; 

                error = read_data(VOC);
                if (error==DATA_STATE_OK) *VOC /= 10; 

                error = read_data(NOx);
                if (error==DATA_STATE_OK) *NOx /= 10; 

                return true;
            }
            
            template <class Unsigned_type, class Signed_type>
            bool get_measured_raw_values(Unsigned_type* mc_pm1p0, Unsigned_type* mc_pm2p5,
                                        Unsigned_type* mc_pm4p0, Unsigned_type* mc_pm10, 
                                        Signed_type* CAH, Signed_type* CAT, Signed_type* VOC,
                                        Signed_type* NOx)
            {
                if (__SEN_STATE__ == SEN_5x_IDLE_MODE)
                    return false;

                const uint8_t num_values = 8;

                send_command(READ_MEASURED_VALUES);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*3));

                uint8_t error;

                error = read_data(mc_pm1p0);
                error = read_data(mc_pm2p5);
                error = read_data(mc_pm4p0);
                error = read_data(mc_pm10);
                error = read_data(CAH);
                error = read_data(CAT);
                error = read_data(VOC);
                error = read_data(NOx);

                return true;
            }
            
            bool get_temp_comp_parameters(int16_t* offset, int16_t* norm_offset_slope, uint16_t* time_const)
            {
                const uint8_t num_values = 3;

                send_command(READ_WRITE_TEMP_COMP_PARAMETERS);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*3));

                uint8_t error;

                error = read_data(offset);
                error = read_data(norm_offset_slope);
                error = read_data(time_const);

                return true;
            }

            void set_temp_comp_parameters(int16_t* offset, int16_t* norm_offset_slope, uint16_t* time_const)
            {
                send_command(READ_WRITE_TEMP_COMP_PARAMETERS);
                delay(20);

                send_data(*offset);
                send_data(*norm_offset_slope);
                send_data(*time_const);
            }
            
            bool get_warm_start_parameter(uint16_t* warm_start_param)
            {
                const uint8_t num_values = 1;

                send_command(READ_WRITE_WARM_START_PARAMETERS);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*3));

                uint8_t error;

                error = read_data(warm_start_param);

                return true;
            }

            void set_warm_start_parameter(uint16_t* offset)
            {
                send_command(READ_WRITE_WARM_START_PARAMETERS);
                delay(20);

                send_data(*offset);
            }

            bool get_VOC_alg_tuning_parameters(int16_t* index_offset, int16_t* LT_offset_hrs,
                                                int16_t* LT_gain_hrs, int16_t* gating_max_minutes,
                                                int16_t* std_initial, int16_t* gain_factor)
            {
                const uint8_t num_values = 6;

                send_command(READ_WRITE_VOC_ALG_TUNING_PARAMETERS);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*3));

                uint8_t error;

                error = read_data(index_offset);
                error = read_data(LT_offset_hrs);
                error = read_data(LT_gain_hrs);
                error = read_data(gating_max_minutes);
                error = read_data(std_initial);
                error = read_data(gain_factor);

                return true;
            }

            void set_VOC_alg_tuning_parameters(int16_t* index_offset, int16_t* LT_offset_hrs,
                                                int16_t* LT_gain_hrs, int16_t* gating_max_minutes,
                                                int16_t* std_initial, int16_t* gain_factor)
            {
                send_command(READ_WRITE_VOC_ALG_TUNING_PARAMETERS);
                delay(20);

                send_data(*index_offset);
                send_data(*LT_offset_hrs);
                send_data(*LT_gain_hrs);
                send_data(*gating_max_minutes);
                send_data(*std_initial);
                send_data(*gain_factor);
            }

            bool get_NOX_alg_tuning_parameters(int16_t* index_offset, int16_t* LT_offset_hrs,
                                                int16_t* LT_gain_hrs, int16_t* gating_max_minutes,
                                                int16_t* std_initial, int16_t* gain_factor)
            {
                const uint8_t num_values = 6;

                send_command(READ_WRITE_NOX_ALG_TUNING_PARAMETERS);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*3));

                uint8_t error;

                error = read_data(index_offset);
                error = read_data(LT_offset_hrs);
                error = read_data(LT_gain_hrs);
                error = read_data(gating_max_minutes);
                error = read_data(std_initial);
                error = read_data(gain_factor);

                return true;
            }

            void set_NOX_alg_tuning_parameters(int16_t* index_offset, int16_t* LT_offset_hrs,
                                                int16_t* LT_gain_hrs, int16_t* gating_max_minutes,
                                                int16_t* std_initial, int16_t* gain_factor)
            {
                send_command(READ_WRITE_NOX_ALG_TUNING_PARAMETERS);
                delay(20);

                send_data(*index_offset);
                send_data(*LT_offset_hrs);
                send_data(*LT_gain_hrs);
                send_data(*gating_max_minutes);
                send_data(*std_initial);
                send_data(*gain_factor);
            }

            bool get_RHT_acceleration_mode(uint16_t* RHT_accel_mode)
            {
                const uint8_t num_values = 1;

                send_command(READ_WRITE_RHT_ACCELERATION_MODE);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*3));

                uint8_t error;

                error = read_data(RHT_accel_mode);

                return true;
            }

            void set_RHT_acceleration_mode(uint16_t* accel_mode)
            {
                send_command(READ_WRITE_RHT_ACCELERATION_MODE);
                delay(20);

                send_data(*accel_mode);
            }

            bool get_VOC_alg_state(uint32_t* VOC_alg_state)
            {
                const uint8_t num_values = 2;

                send_command(READ_WRITE_VOC_ALG_STATE);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*2));

                uint16_t chunk;
                uint8_t error;

                error = read_data(&chunk);

                *VOC_alg_state = (uint32_t)((chunk << 16) & 0xFFFF);

                error = read_data(&chunk);

                *VOC_alg_state = *VOC_alg_state | (chunk | 0x0000);

                return true;
            }

            void set_VOC_alg_state(uint32_t* state)
            {
                send_command(READ_WRITE_VOC_ALG_STATE);
                delay(20);

                send_data((*state >> 16) & 0x00FF);
                send_data((*state & 0x00FF));
            }

            bool start_fan_cleaning_cycle()
            {
                if (__SEN_STATE__ != SEN_5x_MEASUREMENT_MODE)
                    return false;
                
                send_command(START_FAN_CLEANING);
                delay(20);

                return true;
            }

            bool get_auto_cleaning_interval(uint32_t* auto_cleaning_interval)
            {
                const uint8_t num_values = 2;

                send_command(READ_WRITE_AUTO_CLEANING_INITERVAL);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*2));

                uint16_t chunk;
                uint8_t error;

                error = read_data(&chunk);

                *auto_cleaning_interval = (uint32_t)((chunk << 16) & 0xFFFF);

                error = read_data(&chunk);

                *auto_cleaning_interval = *auto_cleaning_interval | (chunk | 0x0000);

                return true;
            }

            void set_auto_cleaning_interval(uint32_t* seconds)
            {
                send_command(READ_WRITE_AUTO_CLEANING_INITERVAL);
                delay(20);

                send_data((*seconds >> 16) & 0x00FF);
                send_data((*seconds & 0x00FF));
            }

            bool get_product_name(char buf[32])
            {
                const uint8_t num_values = 16;

                send_command(READ_PRODUCT_NAME);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*3));

                uint16_t i_buf;
                
                uint8_t error;

                for (uint8_t pair_num = 0; pair_num < num_values; ++pair_num){
                    error = read_data(&i_buf);

                    buf[pair_num*2] = (char)(i_buf >> 8);
                    buf[pair_num*2+1] = (char)(i_buf & 0x00FF);
                }

                return true;
            }

            bool get_serial_number(char buf[32])
            {
                const uint8_t num_values = 16;

                send_command(READ_SERIAL_NUMBER);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*3));

                uint16_t i_buf;
                
                uint8_t error;

                for (uint8_t pair_num = 0; pair_num < num_values; ++pair_num){
                    error = read_data(&i_buf);

                    buf[pair_num*2] = (char)(i_buf >> 8);
                    buf[pair_num*2+1] = (char)(i_buf & 0x00FF);
                }

                return true;
            }

            bool get_firmware_version(uint8_t* version)
            {
                uint16_t tmp = *version;
                bool good = get_firmware_version(&tmp);
                *version = tmp;

                return good;
            }

            bool get_firmware_version(uint16_t* version)
            {
                const uint8_t num_values = 1;

                send_command(READ_FIRMWARE_VERSION);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*3));

                uint8_t error;

                error = read_data(version);
                if (error == DATA_STATE_OK)
                {
                    return true;
                }

                return false;
            }

            bool get_device_status(uint32_t* device_status)
            {
                const uint8_t num_values = 2;

                send_command(READ_WRITE_AUTO_CLEANING_INITERVAL);
                delay(20);

                Wire.requestFrom(SEN_54_ADDRESS, uint8_t(num_values*2));

                uint16_t chunk;
                uint8_t error;

                error = read_data(&chunk);

                *device_status = (uint32_t)(chunk << 16);

                error = read_data(&chunk);

                *device_status = *device_status | chunk;

                return true;
            }

            void clear_device_status()
            {
                send_command(CLEAR_DEVICE_STATUS);
                delay(20);
            }
            
            void reset()
            {
                send_command(RESET);
                delay(100);

                __SEN_STATE__ = SEN_5x_IDLE_MODE;
        }
    };
}
#endif //SEN_5X_HEADER
