#include <bsp.h>
#include <esp32.h>
#include <canbus.h>
#include <common.h>
#include <bsp_time.h>
#include <i2c_driver.h>

#define BUFFER_SIZE 128

static void handle_request(void);
static void handle_receive(size_t length);

static volatile uint8_t tx_counter = 0;
static uint8_t tx_buffer[BUFFER_SIZE] = {};

void esp32_init(void)
{
    uint8_t status = i2c_driver_init(handle_request, handle_receive);
    status = (status == OKAY) ? OKAY : I2C_ERROR;
    set_esp32_status(status);
}

void esp32_run(void)
{
    if (OKAY == get_rtc_status())
    {
        set_rtc_year(bsp_year());
        set_rtc_month(bsp_month());
        set_rtc_day(bsp_day());
        set_rtc_hour(bsp_hour());
        set_rtc_minute(bsp_minute());
        set_rtc_second(bsp_second());
    }
}

static void handle_receive(size_t length)
{
    uint8_t status = I2C_ERROR;
    char data[length + 1] = {};

    if (OKAY == i2c_driver_read((uint8_t *)data, length))
    {
        if (length == 20U)
        {
            int _year = 0, _month = 0, _minute = 0, _second = 0, _hour = 0, _day = 0;
            sscanf(data, "%04d-%02d-%02d %02d:%02d:%02d", &_year, &_month, &_day, &_hour, &_minute, &_second);

            if (_year >= 2020 && (_month >= 1 && _month <= 12) && (_day >= 1 && _day <= 31) && (_minute > 0 && _minute < 60) && (_hour > 0 && _hour < 24) && (_second > 0 && _second < 60))
            {
                // set the RTC datetime
                bsp_set_time(_year, _month, _day, _hour, _minute, _second);
                status = OKAY;
            }
        }
    }

    set_esp32_status(status);
}

static void handle_request(void)
{
    uint8_t status = I2C_ERROR;

    // if (tx_buffer[tx_counter] == '\n') //  doubt how to set CAN signals to the buffer???

    uint8_t length = strlen((char *)tx_buffer) + 1;

    if (i2c_driver_write(tx_buffer, length))
    {
        status = OKAY;
    }

    set_esp32_status(status);
}
