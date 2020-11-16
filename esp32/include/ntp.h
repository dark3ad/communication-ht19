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

#define DATE_TIME_LENGTH 20

/**
 * @brief This function is used to connect the wifi, to initailize i2c driver 
 *         and to configure the ntp server
 */
void ntp_init(void);

/**
 * @brief This function is used to send the date time to the teensy via 12c
 *         and also the status message if there.
 * 
 */
void ntp_run(void);

#endif /* NTP_H */