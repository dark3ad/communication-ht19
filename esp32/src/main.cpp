#include <bsp.h>
#include <time.h>
#include <common.h>
#include <bsp_time.h>
#include <i2c_driver.h>
#include <led_driver.h>
#include <wifi_driver.h>

void setup()
{
    bsp_serial_begin();

    led_driver_init();

    if (!i2c_driver_init())
    {
        while (true)
        {
            // blink the led
            bsp_delay(500);
        }
    }

    wifi_driver_init();
    wifi_driver_connect();

    uint8_t status = NTP_ERROR;
    while (status != OKAY)
    {
        //NTP init and get the time
        configTime(TIME_OFFSET, TIME_DAYLIGHT, NTP_ADDRESS);

        // get time
        // if time is not okay then send ntp error
        // else send date and time to teensy and set status to okay
    }
}

void loop()
{
}