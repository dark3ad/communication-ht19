/**
 * @file ntp.h
 * @author PraveenaSiva (pravisiva757@gmail.com)
 * @brief  NTP module in esp32 to set RTC in Teensy via i2c
 * @version 0.1
 * @date 2020-11-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef NTP_H
#define NTP_H

#include <Arduino.h>
#include <stdint.h>

#define SSID "comhem_E7C457"  //"YA-OPEN"
#define PASSWORD "40430DBB28" //"utbildning2015"
#define DATE_TIME_LENGTH 20
#define ERROR (0U)
#define OKAY (1U)

/**
 * @brief This function is used to connect the wifi, to initailize i2c driver 
 *         and to configure the ntp server
 * @return uint8_t status. It can be OKAY or ERROR
 */
uint8_t ntp_datetime_init(void);

/**
 * @brief Get the ntp datetime object
 * @param uint8_t * variable in which datatime is stored
 * @return uint8_t status. It can be OKAY or ERROR 
 */
uint8_t get_ntp_datetime(uint8_t *);

/**
 * @brief This function is used to write the date time (from NTP) to the i2c slave(Teensy)
 * @param uint8_t * variable which contain date time which is supposed to be write.
 * @return uint8_t status. It can be OKAY or ERROR 
 */
uint8_t send_ntp_datetime(uint8_t *);

#endif /* NTP_H */