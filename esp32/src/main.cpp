#include <bsp.h>
#include <time.h>
#include <common.h>
#include <bsp_time.h>
#include <i2c_driver.h>
#include <led_driver.h>
#include <wifi_driver.h>

#define SSID "YA-OPEN"
#define PASS "utbildning2015"
#define DATE_TIME_LENGTH (19U)

void setup()
{
#ifdef DEVELOPMENT
    bsp_serial_begin();
#endif

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

    wifi_driver_init(SSID, PASS);

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
            if (i2c_driver_write(datetime, DATE_TIME_LENGTH))
            {
                status = OKAY;
            }
        }
        else
        {
            // Send ntp error message to the teensy
            i2c_driver_write(&status, sizeof(status));
        }

        bsp_delay(1000);
    }
}

void loop()
{
}