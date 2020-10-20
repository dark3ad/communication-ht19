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
1.  <wifi_status>     2.  unsigned int    3.  7 bits    4.  0 - 3    5.  wifi status connected(0),disconnected(1),connection_failed(2),connecting(3) values from ESP32      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <mqtt_status>     2.  unsigned int    3.  7 bits    4.  0 - 3    5.  mqtt status connected(0),disconnected(1),connection_failed(2),connecting(3) values from broker      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <i2c_status>     2.  unsigned int    3.  7 bits    4.  0 - 3    5.  i2c status connected(0),disconnected(1),connection_failed(2),connecting(3) values from ESP32      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <ntp_status>     2.  unsigned int    3.  7 bits    4.  0 - 1    5.  RTC status uninitialized(0), initialized(1) values from ESP32/NTP-req      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <date_time_now>     2.  unsigned int    3.  7 bits    4.  0    5.  Date time distribution from NTP-server to CANbus(0)      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <terminal_status>     2.  unsigned int    3.  7 bits    4.  0-1    5.  Terminal mode connected(0), disconnected(1)      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <log_manager_status>     2.  unsigned int    3.  7 bits    4.  0-2    5.  Terminal mode uninitialized(0), initialized(1), error(2)      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <led_status>     2.  unsigned int    3.  7 bits    4.  0    5.  BUILDIN LED status on(0) off(1)       6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <sdcard_status>     2.  unsigned int    3.  7 bits    4.  0-2    5.  SD-card status uninitialized(0), initialized(1), error(2) to CANbus      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <free_mem>     2.  unsigned int    3.  7 bits    4.  0-1    5.  SD-card available memory broadcast to CANbus      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <log_manager_err>     2.  unsigned int    3.  7 bits    4.  0    5.  Log manager (internal) errors write to specific error.log file      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <critical_err>     2.  unsigned int    3.  7 bits    4.  0    5.  Critical error detection turns BUILDIN LED on      6.  No     7.  No      8.  Communication/CAN         9.  Communication/CAN              10. 1000ms
1.  <set_temperature_min_cal_value>     2.  unsigned int    3.  7 bits  4.  0 - 50 grader celsius   5.  set minimum temperature calibration value to CAN      6.  Yes     7.  Yes      8.  Sensor         9.  HMI/Actuators/Comm              10. 1000ms
    <set_temperature_max_cal_value>         unsigned int        7 bits      0 - 50 grader celsius       set maximum temperature calibration value to CAN          Yes         Yes          Sensor             HMI/Actuators/Comm                  1000ms
    <set_humidity_min_cal_value>            unsigned int        7 bits      0 - 100                     set minimum humidity calibration value to CAN             Yes         Yes          Sensor             HMI/Actuators/Comm                  1000ms
    <set_humidity_max_cal_value>            unsigned int        7 bits      0 - 100                     set maximum humidity calibration value to CAN             Yes         Yes          Sensor             HMI/Actuators/Comm                  1000ms
1.  <set_flow_meter_min_cal_value>      2.  unsigned int    3.  9 bits  4.  17 - 500 mL/s           5.  set minimum flow meter calibration value to CAN       6.  Yes     7.  Yes      8.  Sensor         9.  HMI/Actuators/Comm              10. 1000ms
    <set_flow_meter_max_cal_value>          unsigned int        9 bits      17 - 500 mL/s               set maximum flow meter calibration value to CAN           Yes         Yes          Sensor             HMI/Actuators/Comm                  1000ms
1.  <set_ldr_min_cal_value>             2.  unsigned int    3.  7 bits  4.  0 - 100                 5.  set minimum ldr calibration value to CAN              6.  Yes     7.  Yes      8.  Sensor         9.  HMI/Actuators/Comm              10. 1000ms
    <set_ldr_max_cal_value>                 unsigned int        7 bits      0 - 100                     set maximum ldr calibration value to CAN                  Yes         Yes          Sensor             HMI/Actuators/Comm                  1000ms
1.  <set_soil_moisture_min_cal_value>   2.  unsigned int    3.  7 bits  4.  0 - 100                 5.  set minimum soil moisture calibration value to CAN    6.  Yes     7.  Yes      8.  Sensor         9.  HMI/Actuators/Comm              10. 1000ms
    <set_soil_moisture_max_cal_value>       unsigned int        7 bits      0 - 100                     set maximum soil moisture calibration value to CAN        Yes         Yes          Sensor             HMI/Actuators/Comm                  1000ms
1.  <set_water_level_min_cal_value>     2.  unsigned int    3.  7 bits  4.  0 - 100                 5.  set minimum water level calibration value to CAN      6.  Yes     7.  Yes      8.  Sensor         9.  HMI/Actuators/Comm              10. 1000ms
    <set_water_level_max_cal_value>         unsigned int        7 bits      0 - 100                     set maximum water level calibration value to CAN          Yes         Yes          Sensor             HMI/Actuators/Comm                  1000ms
1.  <set_light_intensity_min_cal_value>     2.  unsigned int    3.  7 bits  4.  0 - 100   5.  set minimum light intensity calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_light_intensity_max_cal_value>     2.  unsigned int    3.  7 bits  4.  0 - 100   5.  set maximum light intensity calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_lamp_status>     2.  unsigned int    3.  7 bits  4.  0 - 1   5.  set status of lamp ON(0), OFF(1) value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_soilmoisture_min_cal_value>     2.  unsigned int    3.  7 bits  4.  0 - 100   5.  set minimum soilmoisture calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_soilmoisture_max_cal_value>     2.  unsigned int    3.  7 bits  4.  0 - 100   5.  set maximum soilmoisture calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <waterpump_status>     2.  unsigned int    3.  7 bits  4.  0 - 1   5.  set status of waterpump ON(0), OFF(1) value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_liquid_levele_min_cal_value>     2.  unsigned int    3.  7 bits  4.  0 - 100   5.  set minimum soilmoisture calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_liquid_levele_max_cal_value>     2.  unsigned int    3.  7 bits  4.  0 - 100   5.  set maximum soilmoisture calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_watervalve_status>     2.  unsigned int    3.  7 bits  4.  0 - 1   5.  set status of watervalve ON(0), OFF(1) value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_window_status>     2.  unsigned int    3.  7 bits  4.  0 - 1   5.  set status of window ON(0), OFF(1) value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_fan_status>     2.  unsigned int    3.  7 bits  4.  0 - 1   5.  set status of fan ON(0), OFF(1) value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_heater_status>     2.  unsigned int    3.  7 bits  4.  0 - 1   5.  set status of heater ON(0), OFF(1) value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_temperature_min_cal_value>     2.  unsigned int    3.  7 bits  4.  UNKNOWN(CONST?)   5.  set minimum temperature calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_temperature_max_cal_value>     2.  unsigned int    3.  7 bits  4.  UNKNOWN(CONST?)   5.  set maximum temperature calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_humidity_min_cal_value>     2.  unsigned int    3.  7 bits  4.  UNKNOWN(CONST?)   5.  set minimum humidity calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_humidity_max_cal_value>     2.  unsigned int    3.  7 bits  4.  UNKNOWN(CONST?)   5.  set maximum humidity calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_soilmoisture_min_cal_value>     2.  unsigned int    3.  7 bits  4.  UNKNOWN(CONST?)   5.  set minimum soilmoisture calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_soilmoisture_max_cal_value>     2.  unsigned int    3.  7 bits  4.  UNKNOWN(CONST?)   5.  set maximum soilmoisture calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_intensity_min_cal_value>     2.  unsigned int    3.  7 bits  4.  UNKNOWN(CONST?)   5.  set minimum intensity calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_intensity_max_cal_value>     2.  unsigned int    3.  7 bits  4.  UNKNOWN(CONST?)   5.  set maximum intensity calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_liquid_min_cal_value>     2.  unsigned int    3.  7 bits  4.  UNKNOWN(CONST?)   5.  set minimum liquid calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms
1.  <set_liquid_max_cal_value>     2.  unsigned int    3.  7 bits  4.  UNKNOWN(CONST?)   5.  set maximum liquid calibration value to CAN      6.  Yes     7.  Yes      8.  Actuators         9.  HMI/Actuators/Comm              10. 1000ms

### END OF FILE
