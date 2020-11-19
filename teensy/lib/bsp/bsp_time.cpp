#include <TimeLib.h>
#include <Arduino.h>
#include <bsp_time.h>
#include <TimeLib.h>

void bsp_delay(uint32_t msec)
{
    delay(msec);
}

uint16_t bsp_year(void)
{
    return (uint16_t)year();
}

uint8_t bsp_month(void)
{
    return (uint8_t)month();
}

uint8_t bsp_day(void)
{
    return (uint8_t)day();
}

uint8_t bsp_hour(void)
{
    return (uint8_t)hour();
}

uint8_t bsp_minute(void)
{
    return (uint8_t)minute();
}

uint8_t bsp_second(void)
{
    return (uint8_t)second();
}

void bsp_set_time(uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, uint8_t _second)
{
    setTime(_hour, _minute, _second, _day, _month, _year);
}
