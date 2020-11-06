

#include <bsp.h>
#include "esp32.h"
#include <i2c_t3.h>
#include <i2c_driver.h>
#include <TimeLib.h>

#define DATE_TIME_LENGTH 20

#define YEAR_LENGTH 4
#define YEAR_OFFSET 0

#define MONTH_LENGTH 2
#define MONTH_OFFSET (YEAR_OFFSET + YEAR_LENGTH + 1) // 5

#define DAY_LENGTH 2
#define DAY_OFFSET (MONTH_OFFSET + MONTH_LENGTH + 1) // 8

#define HOUR_LENGTH 2
#define HOUR_OFFSET (DAY_OFFSET + DAY_LENGTH + 1) // 11

#define MINUTE_LENGTH 2
#define MINUTE_OFFSET (HOUR_OFFSET + HOUR_LENGTH + 1) // 14

#define SECOND_LENGTH 2
#define SECOND_OFFSET (MINUTE_OFFSET + MINUTE_LENGTH + 1) // 17

#define BUFFER_SIZE 128
static volatile uint8_t tx_counter = 0;
static uint8_t tx_buffer[BUFFER_SIZE] = {};

static void requestEvent(void);
static void receiveEvent(size_t size);

uint8_t RTC_init(void (*request)(void), void (*recieve)(size_t))
{
    request = requestEvent;
    recieve = receiveEvent;

    return i2c_driver_init(request, recieve);
}

datetime_t strtotime(char *buffer)
{
    char temp[YEAR_LENGTH + 1] = {};
    datetime_t datetime = {.year = 0, .month = 0, .day = 0, .hour = 0, .minute = 0, .second = 0};

    memcpy(temp, buffer + YEAR_OFFSET, YEAR_LENGTH);
    datetime.year = atoi(temp);

    memset(temp, 0, YEAR_LENGTH);
    memcpy(temp, buffer + MONTH_OFFSET, MONTH_LENGTH);
    datetime.month = atoi(temp);

    memset(temp, 0, MONTH_LENGTH);
    memcpy(temp, buffer + DAY_OFFSET, DAY_LENGTH);
    datetime.day = atoi(temp);

    memset(temp, 0, DAY_LENGTH);
    memcpy(temp, buffer + HOUR_OFFSET, HOUR_LENGTH);
    datetime.hour = atoi(temp);

    memset(temp, 0, HOUR_LENGTH);
    memcpy(temp, buffer + MINUTE_OFFSET, MINUTE_LENGTH);
    datetime.minute = atoi(temp);

    memset(temp, 0, MINUTE_LENGTH);
    memcpy(temp, buffer + SECOND_OFFSET, SECOND_LENGTH);
    datetime.second = atoi(temp);

    return datetime;
}

static void requestEvent(void)
{
    if (tx_buffer[tx_counter] == '\n')
    {
        uint8_t length = strlen((char *)tx_buffer) + 1;

        if (!i2c_driver_write(tx_buffer, length))
        {
            Serial.println("Failed to send the data!");
        }
        memset(tx_buffer, 0, BUFFER_SIZE);
        tx_counter = 0;
    }
}

static void receiveEvent(size_t size)
{

    uint8_t data[DATE_TIME_LENGTH] = {};

    if (i2c_driver_read(data, size))
    {
        Serial.printf("\nMessage :  %s ", data);
        if (size == DATE_TIME_LENGTH)
        {
            datetime_t datetime = strtotime((char *)data);
            setTime(datetime.hour, datetime.minute, datetime.second, datetime.day, datetime.month, datetime.year);

            Serial.print("\nSetting RTC time ..... ");
            Serial.print("\nRTC datetime:   ");
            Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n\n", year(), month(), day(), hour(), minute(), second());
        }
    }
}
