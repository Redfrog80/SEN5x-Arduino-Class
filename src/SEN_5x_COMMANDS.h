#ifndef SEN_5X_COMMANDS
#define SEN_5X_COMMANDS
/*Starts the measurement. After power up, the module is in Idle-Mode. Before any measurement values can be read, the
Measurement-Mode needs to be started using this command.*/
#define SEN_5X_START_MEASUREMENT uint16_t(0x0021)
/*Starts a continuous measurement without PM. Only humidity, temperature, VOC, and NOx are available in this mode.
Laser and fan are switched off to keep power consumption low.*/
#define SEN_5X_START_MEASUREMENT_RHT_GAS_ONLY 0x0037
/*Stops the measurement. Use this command to return to the initial state (Idle-Mode).*/
#define SEN_5X_STOP_MEASUREMENT 0x0104
/*This command can be used for polling to find out when new measurements are available.

READ/WRITE DATA:

Byte [0]:
Description: unused, always 0x00

Byte [1]:
Description: Data-Ready Flag
0x00: no new measurements available
0x01: new measurements ready to be read

Byte [2]:
Description: checksum for bytes [0:1]

*/
#define SEN_5X_READ_DATA_READY_FLAG 0x0202
/*Reads the measured values from the sensor module and resets the "Data-Ready Flag". If the sensor module is in
Measurement-Mode, an updated measurement value is provided every second and the "Data-Ready Flag" is set. If no
synchronized readout is desired, the "Data-Ready Flag" can be ignored. The command "Read Measured Values" always
returns the latest measured values. After sending the "Read Measured Values" command, a wait time of 10 ms is needed
so that the sensor internally can fill the data buffers. After 20 ms, the read data header can be sent to read out the sensor
data. In RHT/Gas-Only Measurement Mode, the PM output is 0xFFFF. If any value is unknown, 0xFFFF is returned.

READ/WRITE DATA:

Bytes [0:1]: 
Type: uint16, 
Scale factor: 10, 
Description: Mass Concentration PM1.0 [μg/m³] 

Byte [2]: 
Description: checksum for [0:1]

Bytes [3:4]: 
Type: uint16,
Scale factor: 10, 
Description: Mass Concentration PM2.5 [μg/m³]

Byte [5]:
Description: checksum for [3:4]

Bytes [6:7]: 
Type: uint16, 
Scale factor: 10, 
Description: Mass Concentration PM4.0 [μg/m³]

Byte [8]:
Description: checksum for [6:7]

Bytes [9:10]: 
Type: uint16, 
Scale factor: 10, 
Description: Mass Concentration PM10 [μg/m³]

Byte [11]:
Description: checksum for [9:10]

Bytes [12:13]: 
Type: int16, 
Scale factor: 100, 
Description: Compensated Ambient Humidity [%RH]

Byte [14]:
Description: checksum for [12:13]

Bytes [15:16]: 
Type int16, 
Scale factor: 200, 
Description: Compensated Ambient Temperature [°C]

Byte [17]:
Description: checksum for [15:16]

Bytes [18:19]: 
Type: int16, 
Scale factor: 10, 
Description: VOC Index

Byte [20]:
Description: checksum for [18:19]

Bytes [21:22]: 
Type: int16, 
Scale factor: 10, 
Description: NOx Index

Byte [23]:
Description: checksum for [21:22]
*/
#define SEN_5X_READ_MEASURED_VALUES 0x03C4
/*These commands allow to compensate temperature effects of the design-in at customer side by applying a custom
temperature offset to the ambient temperature. The compensated ambient temperature is calculated as follows:

T_Ambient_Compensated = T_Ambient + (slope*T_Ambient) + offset

Where slope and offset are the values set with this command, smoothed with the specified time constant. The time
constant is how fast the slope and offset are applied. After the specified value in seconds, 63% of the new slope
and offset are applied.

More details about the tuning of these parameters are included in the application note "Temperature Acceleration and
Compensation Instructions for SEN5x".

All temperatures (T_Ambient_Compensated,T_Ambient and offset) are represented in °C.

READ/WRITE DATA AND PARAMETERS:

Bytes [0:1]: 
Type: int16, 
Scale factor: 200, 
Description: Temperature offset [°C] (default value: 0)

Byte [2]: 
Description: checksum for [0:1]

Bytes [3:4]: 
Type: int16, 
Scale factor: 10000, 
Description: Normalized temperature offset slope (default value: 0)

Byte [5]: 
Description: checksum for [3:4]

Bytes [6:7]: 
Type: uint16, 
Scale factor: 10000, 
Description: Time constant in seconds (default value: 0)

Byte [8]: 
Description: checksum for [6:7]
*/
#define SEN_5X_READ_WRITE_TEMP_COMP_PARAMETERS 0x60B2
/*The temperature compensation algorithm is optimized for a cold start by default, i.e., it is assumed that the "Start
Measurement" commands are called on a device not yet warmed up by previous measurements. If the measurement is
started on a device that is already warmed up, this parameter can be used to improve the initial accuracy of the
ambient temperature output. This parameter can be gotten and set in any state of the device, but it is applied only the
next time starting a measurement, i.e., when sending a "Start Measurement" command. So, the parameter needs to
be written before a warm-start measurement is started.

READ/WRITE DATA AND PARAMETERS:

Bytes [0:1]: 
Type: uint16, 
Scale factor: 1, 
Description: Warm start behavior as a value in the range from
0 (cold start, default value) to 65535 (warm start).
(default value: 0)

Byte [2]: 
Description: checksum for [0:1]
*/
#define SEN_5X_READ_WRITE_WARM_START_PARAMETERS 0x60C6
/*The VOC algorithm can be customized by tuning 6 different parameters. More details on the tuning instructions are
provided in the application note "Engineering Guidelines for SEN5x". Note that this command is available only in idle
mode. In measure mode, this command has no effect. In addition, it has no effect if at least one parameter is outside
the specified range.

READ/WRITE DATA AND PARAMETERS:

Bytes [0:1]: 
Type: int16, 
Scale factor: 1, 
Description: Index Offset
VOC index representing typical (average) conditions. Allowed
values are in range 1..250. The default value is 100.

Byte [2]: 
Description: checksum for [0:1]

Bytes [3:4]: 
Type: int16, 
Scale factor: 1, 
Description: Learning Time Offset Hours
Time constant to estimate the VOC algorithm offset from the
history in hours. Past events will be forgotten after about twice the
learning time. Allowed values are in range 1..1000. The default
value is 12 hours.

Byte [5]: 
Description: checksum for [3:4]

Bytes [6:7]: 
Type: int16, 
Scale factor: 1, 
Description: Learning Time Gain Hours
Time constant to estimate the VOC algorithm gain from the history
in hours. Past events will be forgotten after about twice the
learning time. Allowed values are in range 1..1000. The default
value is 12 hours.

Byte [8]: 
Description: checksum for [6:7]

Bytes [9:10]: 
Type: int16, 
Scale factor: 1, 
Description: Gating Max Duration Minutes
Maximum duration of gating in minutes (freeze of estimator during
high VOC index signal). Zero disables the gating. Allowed values
are in range 0..3000. The default value is 180 minutes.

Byte [11]: 
Description: checksum for [9:10]

Bytes [12:13]: 
Type: int16, 
Scale factor: 1, 
Description: Std Initial
Initial estimate for standard deviation. Lower value boosts events
during initial learning period, but may result in larger device-todevice
variations. Allowed values are in range 10..5000. The
default value is 50.

Byte [14]: 
Description: checksum for [12:13]

Bytes [15:16]: 
Type: int16, 
Scale factor: 1, 
Description: Gain Factor
Gain factor to amplify or to attenuate the VOC index output.
Allowed values are in range 1..1000. The default value is 230.

Byte [17]: 
Description: checksum for [15:16]
*/
#define SEN_5X_READ_WRITE_VOC_ALG_TUNING_PARAMETERS 0x60D0
/*The NOx algorithm can be customized by tuning 6 different parameters. More details on the tuning instructions are
provided in the application note "Engineering Guidelines for SEN5x". This command is available only in idle mode. In
measure mode, this command has no effect. In addition, it has no effect if at least one parameter is outside the
specified range.

READ/WRITE DATA AND PARAMETERS:

Bytes [0:1]: 
Type: int16, 
Scale factor: 1, 
Description: Index Offset
NOx index representing typical (average) conditions. Allowed
values are in range 1..250. The default value is 1.

Byte [2]: 
Description: checksum for [0:1]

Bytes [3:4]: 
Type: int16, 
Scale factor: 1, 
Description: Learning Time Offset Hours
Time constant to estimate the NOx algorithm offset from the
history in hours. Past events will be forgotten after about twice the
learning time. Allowed values are in range 1..1000. The default
value is 12 hours.

Byte [5]: 
Description: checksum for [3:4]

Bytes [6:7]: 
Type: int16, 
Scale factor: 1, 
Description: Learning Time Gain Hours
The time constant to estimate the NOx algorithm gain from the
history has no impact for NOx. This parameter is still in place for
consistency reasons with the VOC tuning parameters command.
This parameter must always be set to 12 hours.

Byte [8]: 
Description: checksum for [6:7]

Bytes [9:10]: 
Type: int16, 
Scale factor: 1, 
Description: Gating Max Duration Minutes
Maximum duration of gating in minutes (freeze of estimator during
high NOx index signal). Set to zero to disable the gating. Allowed
values are in range 0..3000. The default value is 720 minutes.

Byte [11]: 
Description: checksum for [9:10]

Bytes [12:13]: 
Type: int16, 
Scale factor: 1, 
Description: Std Initial
The initial estimate for standard deviation parameter has no
impact for NOx. This parameter is still in place for consistency
reasons with the VOC tuning parameters command. This
parameter must always be set to 50.

Byte [14]: 
Description: checksum for [12:13]

Bytes [15:16]: 
Type: int16, 
Scale factor: 1, 
Description: Gain Factor
Gain factor to amplify or to attenuate the NOx index output.
Allowed values are in range 1..1000. The default value is 230.

Byte [17]: 
Description: checksum for [15:16]
*/
#define SEN_5X_READ_WRITE_NOX_ALG_TUNING_PARAMETERS 0x60E1
/*By default, the RH/T acceleration algorithm is optimized for a sensor which is positioned in free air. If the sensor is
integrated into another device, the ambient RH/T output values might not be optimal due to different thermal behavior.
This parameter can be used to adapt the RH/T acceleration behavior for the actual use-case, leading in an
improvement of the ambient RH/T output accuracy. There is a limited set of different modes available, each identified
by a number:

• 0: Default / Air Purifier / IAQ (slow)
• 1: IAQ (fast)
• 2: IAQ (medium)

This parameter can be changed in any state of the device, but it is applied only the next time starting a measurement,
i.e. when sending a "Start Measurement" command. So, the parameter needs to be set before a new measurement is
started.

READ/WRITE DATA AND PARAMETERS:

Bytes [0:1]: 
Type: int16, 
Scale factor: 1, 
Description: RH/T acceleration mode.

Byte [2]: 
Description: checksum for [0:1]
*/
#define SEN_5X_READ_WRITE_RHT_ACCELERATION_MODE 0x60F7
/*Allows to backup and restore the VOC algorithm state to resume operation after a short interruption, skipping initial
learning phase. By default, the VOC algorithm resets its state to initial values each time a measurement is started,
even if the measurement was stopped only for a short time. So, the VOC index output value needs a long time until it is
stable again. This can be avoided by restoring the previously memorized algorithm state before starting the measure
mode.

READ/WRITE DATA AND PARAMETERS

VOC algorithm state is comprised of 8 bytes and 4 checksums (2 bytes per checksum)

*/
#define SEN_5X_READ_WRITE_VOC_ALG_STATE 0x60F7
/*Starts the fan-cleaning manually. This command can only be executed in Measurement-Mode. For more details, note
the explanations given in 4.2 Fan Auto Cleaning.*/
#define SEN_5X_START_FAN_CLEANING 0x5607 
/*Reads/Writes the interval [s] of the periodic fan-cleaning. For more details, note the explanations given in 5.2 Fan Auto
Cleaning. Please note that since this configuration is volatile, it will be reverted to the default value after a device reset.

READ/WRITE DATA AND PARAMETERS:

unsigned 32-bit int: Auto Cleaning Interval [s]
{
Bytes [0:1]:
Description: Most significant byte

Byte [2]:
Description: checksum for [0:1]

Bytes [3:4]:
Description: least significant byte

Byte [5]:
Description: checksum for [3:4]
}
*/
#define SEN_5X_READ_WRITE_AUTO_CLEANING_INITERVAL 0x8004
/*This command returns the product name SEN5x (SEN50, SEN54 or SEN55). It is defined as a string value with a
maximum length of 32 ASCII characters (including terminating null-character).

READ/WRITE DATA AND PARAMETERS:

Bytes [0:1]:
Description: ASCII Characters [0:1]

Byte [2]:
Description: checksum for [0:1]

Bytes [n-1:n]:
Description: ASCII Characters [n-1:n]

Byte [n+1]:
Description: checksum for [n-1:n]

Bytes [45:46]:
Description: ASCII Characters [45:46]

Byte [47]:
Description: checksum for [45:46]
*/
#define SEN_5X_READ_PRODUCT_NAME 0xD014
/*This command returns the requested serial number. It is defined as a string value with a maximum length of 32 ASCII
characters (including terminating null-character).

READ/WRITE DATA AND PARAMETERS:

Bytes [0:1]:
Description: ASCII Characters [0:1]

Byte [2]:
Description: checksum for [0:1]

Bytes [n-1:n]:
Description: ASCII Characters [n-1:n]

Byte [n+1]:
Description: checksum for [n-1:n]

Bytes [45:46]:
Description: ASCII Characters [45:46]

Byte [47]:
Description: checksum for [45:46]*/
#define SEN_5X_READ_SERIAL_NUMBER 0xD033
/*Get firmware version.

READ/WRITE DATA:

Byte [0]:
Description: Firmware version 

Byte [1]:
Description: Reserved

Byte [2]:
Description: checksum for [0:1] */
#define SEN_5X_READ_FIRMWARE_VERSION 0xD100
/*Use this command to read the Device Status Register. For more details, note the explanations given in section 5.4
Device Status Register.

READ/WRITE DATA AND PARAMETERS:

unsigned 32-bit int: Device Status Register
{
Bytes [0:1]:
Description: Most significant byte

Byte [2]:
Description: checksum for [0:1]

Bytes [3:4]:
Description: least significant byte

Byte [5]:
Description: checksum for [3:4]
}
*/
#define SEN_5X_READ_DEVICE_STATUS 0xD206 
/*Clears all flags in device status register.*/
#define SEN_5X_CLEAR_DEVICE_STATUS 0xD210 
/*Device software reset command. After calling this command, the module is in the same state as after a power reset.*/
#define SEN_5X_RESET 0xD304

#endif //SEN_5X_COMMANDS