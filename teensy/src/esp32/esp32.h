#ifndef ESP32_H
#define ESP32_H

#include <stdint.h>
#include <ctype.h>
#include <Arduino.h>

typedef struct
{

    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} datetime_t;

uint8_t RTC_init(void (*request)(void), void (*recieve)(size_t));

datetime_t strtotime(char *buffer);

#endif