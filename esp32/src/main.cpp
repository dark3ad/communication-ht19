#include <bsp.h>
#include <time.h>
#include <bsp_time.h>
#include <common.h>
#include <i2c_driver.h>
#include <led_driver.h>
#include <wifi_driver.h>

// should remove these macros
#define DATE_TIME_LENGTH (19U)

void setup()
{
    bsp_serial_begin();

    led_driver_init();

    if (!i2c_driver_init())
    {
        while (true)
        {
            // blink the led
            led_driver_turn_on();
            bsp_delay(500);
            led_driver_turn_off();
            bsp_delay(500);
        }
    }

    wifi_driver_init();
    wifi_driver_connect();

    //NTP init and get the time
    configTime(TIME_OFFSET, TIME_DAYLIGHT, NTP_ADDRESS);

    uint8_t status = NTP_ERROR;
    while (status != OKAY)
    {
        struct tm timeinfo;
        uint8_t datetime[DATE_TIME_LENGTH + 1] = {};

        // get time
        if (getLocalTime(&timeinfo))
        {
            strftime((char *)datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", &timeinfo);
            // write date time to teensy and set the status to okay
            i2c_driver_write(datetime, DATE_TIME_LENGTH);
            status = OKAY;
        }
        else
        {
            // if time is not okay, send ntp error message to the teensy
            i2c_driver_write((uint8_t *)NTP_ERROR, sizeof(NTP_ERROR));
        }
    }
}

void loop()
{
}