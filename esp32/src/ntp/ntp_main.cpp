/**
 * @file ntp_main.cpp
 * 
 * @author PraveenaSiva (pravisiva757@gmail.com)
 * @brief  NTP module in esp32 to set RTC in Teensy via i2c
 * @version 0.1
 * @date 2020-11-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "ntp.h"

#define BUFFER_SIZE 128

static uint8_t date_time[DATE_TIME_LENGTH] = {};
static uint8_t rx_buffer[BUFFER_SIZE] = {};

void setup()
{
    Serial.begin(9600);
    delay(3000);

    ntp_datetime_init();
}

void loop()
{
    delay(1000);
    if (get_ntp_datetime(date_time))
    {
        Serial.println((char *)date_time);
        send_get_ntp_datetime(date_time);
    }

    /*if (i2c_driver_read(rx_buffer, sizeof(rx_buffer))) // doubt
    {
        Serial.printf("%s", rx_buffer);
        memset(rx_buffer, 0, BUFFER_SIZE);
    }*/
}