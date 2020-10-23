### Signals for communication/can

Follow these steps when defining the signals.

    1. The signal name
    2. The signal type; signed/unsigned integer or float
    3. Length of the signal
    4. The possible values or range
    5. A short description
    6. Is the signal a calibration value or not?
    7. Is the signal an overwrite signal or not?
    8. Which node/nodes can set the signal
    9. Which node/nodes can get the signal
    10. How often the signal is sent/received

## Input
Expl. fetched from the other groups.

## Output
| Signal name  | Signal type  | Length of signal  | value range  | short description  | Calibration val?  | overwrite signal?  | setters  | getters  | how often  |
|---|---|---|---|---|---|---|---|---|---|
| <wifi_status>                       | unsigned int | 7 bits | 0 - 3                 | wifi status connected(0),disconnected(1),connection_failed(2),connecting(3) values from ESP32  | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <mqtt_status>                       | unsigned int | 7 bits | 0 - 3                 | mqtt status connected(0),disconnected(1),connection_failed(2),connecting(3) values from broker | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <i2c_status>                        | unsigned int | 7 bits | 0 - 3                 | i2c status connected(0),disconnected(1),connection_failed(2),connecting(3) values from ESP32   | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <ntp_status>                        | unsigned int | 7 bits | 0 - 1                 | RTC status uninitialized(0), initialized(1) values from ESP32/NTP-req                          | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <date_time_now>                     | unsigned int | 7 bits | 0                     | Date time distribution from NTP-server to CANbus(0)                                            | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <terminal_status>                   | unsigned int | 7 bits | 0-1                   | Terminal mode connected(0), disconnected(1)                                                    | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <log_manager_status>                | unsigned int | 7 bits | 0-2                   | Terminal mode uninitialized(0), initialized(1), error(2)                                       | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <led_status>                        | unsigned int | 7 bits | 0                     | BUILDIN LED status on(0) off(1)                                                                | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <sdcard_status>                     | unsigned int | 7 bits | 0-2                   | SD-card status uninitialized(0), initialized(1), error(2) to CANbus                            | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <free_mem>                          | unsigned int | 7 bits | 0-1                   | SD-card available memory broadcast to CANbus                                                   | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <log_manager_err>                   | unsigned int | 7 bits | 0                     | Log manager (internal) errors write to specific error.log file                                 | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <critical_err>                      | unsigned int | 7 bits | 0                     | Critical error detection turns BUILDIN LED on                                                  | No  | No  | Communication/CAN | Communication/CAN  | 1000ms |
| <set_temperature_min_cal_value>     | unsigned int | 7 bits | 0 - 50 grader celsius | set minimum temperature calibration value to CAN                                               | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_temperature_max_cal_value>     | unsigned int | 7 bits | 0 - 50 grader celsius | set maximum temperature calibration value to CAN                                               | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_humidity_min_cal_value>        | unsigned int | 7 bits | 0 - 100               | set minimum humidity calibration value to CAN                                                  | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_humidity_max_cal_value>        | unsigned int | 7 bits | 0 - 100               | set maximum humidity calibration value to CAN                                                  | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_flow_meter_min_cal_value>      | unsigned int | 9 bits | 17 - 500 mL/s         | set minimum flow meter calibration value to CAN                                                | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_flow_meter_max_cal_value>      | unsigned int | 9 bits | 17 - 500 mL/s         | set maximum flow meter calibration value to CAN                                                | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_ldr_min_cal_value>             | unsigned int | 7 bits | 0 - 100               | set minimum ldr calibration value to CAN                                                       | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_ldr_max_cal_value>             | unsigned int | 7 bits | 0 - 100               | set maximum ldr calibration value to CAN                                                       | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_soil_moisture_min_cal_value>   | unsigned int | 7 bits | 0 - 100               | set minimum soil moisture calibration value to CAN                                             | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_soil_moisture_max_cal_value>   | unsigned int | 7 bits | 0 - 100               | set maximum soil moisture calibration value to CAN                                             | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_water_level_min_cal_value>     | unsigned int | 7 bits | 0 - 100               | set minimum water level calibration value to CAN                                               | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_water_level_max_cal_value>     | unsigned int | 7 bits | 0 - 100               | set maximum water level calibration value to CAN                                               | Yes | Yes | Sensor            | HMI/Actuators/Comm | 1000ms |
| <set_light_intensity_min_cal_value> | unsigned int | 7 bits | 0 - 100               | set minimum light intensity calibration value to CAN                                           | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_light_intensity_max_cal_value> | unsigned int | 7 bits | 0 - 100               | set maximum light intensity calibration value to CAN                                           | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_lamp_status>                   | unsigned int | 7 bits | 0 - 1                 | set status of lamp ON(0), OFF(1) value to CAN                                                  | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_soilmoisture_min_cal_value>    | unsigned int | 7 bits | 0 - 100               | set minimum soilmoisture calibration value to CAN                                              | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_soilmoisture_max_cal_value>    | unsigned int | 7 bits | 0 - 100               | set maximum soilmoisture calibration value to CAN                                              | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <waterpump_status>                  | unsigned int | 7 bits | 0 - 1                 | set status of waterpump ON(0), OFF(1) value to CAN                                             | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_liquid_levele_min_cal_value>   | unsigned int | 7 bits | 0 - 100               | set minimum soilmoisture calibration value to CAN                                              | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_liquid_levele_max_cal_value>   | unsigned int | 7 bits | 0 - 100               | set maximum soilmoisture calibration value to CAN                                              | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_watervalve_status>             | unsigned int | 7 bits | 0 - 1                 | set status of watervalve ON(0), OFF(1) value to CAN                                            | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_window_status>                 | unsigned int | 7 bits | 0 - 1                 | set status of window ON(0), OFF(1) value to CAN                                                | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_fan_status>                    | unsigned int | 7 bits | 0 - 1                 | set status of fan ON(0), OFF(1) value to CAN                                                   | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_heater_status>                 | unsigned int | 7 bits | 0 - 1                 | set status of heater ON(0), OFF(1) value to CAN                                                | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_temperature_min_cal_value>     | unsigned int | 7 bits | UNKNOWN(CONST?)       | set minimum temperature calibration value to CAN                                               | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_temperature_max_cal_value>     | unsigned int | 7 bits | UNKNOWN(CONST?)       | set maximum temperature calibration value to CAN                                               | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_humidity_min_cal_value>        | unsigned int | 7 bits | UNKNOWN(CONST?)       | set minimum humidity calibration value to CAN                                                  | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_humidity_max_cal_value>        | unsigned int | 7 bits | UNKNOWN(CONST?)       | set maximum humidity calibration value to CAN                                                  | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_soilmoisture_min_cal_value>    | unsigned int | 7 bits | UNKNOWN(CONST?)       | set minimum soilmoisture calibration value to CAN                                              | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_soilmoisture_max_cal_value>    | unsigned int | 7 bits | UNKNOWN(CONST?)       | set maximum soilmoisture calibration value to CAN                                              | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_intensity_min_cal_value>       | unsigned int | 7 bits | UNKNOWN(CONST?)       | set minimum intensity calibration value to CAN                                                 | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_intensity_max_cal_value>       | unsigned int | 7 bits | UNKNOWN(CONST?)       | set maximum intensity calibration value to CAN                                                 | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_liquid_min_cal_value>          | unsigned int | 7 bits | UNKNOWN(CONST?)       | set minimum liquid calibration value to CAN                                                    | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |
| <set_liquid_max_cal_value>          | unsigned int | 7 bits | UNKNOWN(CONST?)       | set maximum liquid calibration value to CAN                                                    | Yes | Yes | Actuators         | HMI/Actuators/Comm | 1000ms |



 
### END OF FILE
