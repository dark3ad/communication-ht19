#include <ntp.h>
#include <common.h>
#include <wifi_driver.h>
#include <i2c_driver.h>
#include "time.h"

#define TIME_OFFSET (1 * 60 * 60)
#define TIME_DAYLIGHT (1 * 60 * 60)
#define NTP_ADDRESS "pool.ntp.org"

void ntp_init(void)
{

    uint8_t status = i2c_driver_init();
    status = (status == OKAY) ? OKAY : I2C_ERROR;

    //connect to WiFi
    wifi_driver_init();
    wifi_driver_connect();

    //NTP init and get the time
    configTime(TIME_OFFSET, TIME_DAYLIGHT, NTP_ADDRESS);

    // set_esp32_status(status);    ??
}

void ntp_run(void)
{
    uint8_t status;
    uint8_t datetime[DATE_TIME_LENGTH] = {};

    struct tm timeinfo;

    if (wifi_driver_status())
    {
        if (getLocalTime(&timeinfo))
        {
            strftime((char *)datetime, DATE_TIME_LENGTH, "%Y-%m-%d %H:%M:%S", &timeinfo);
            // send date time to the teensy
            status = i2c_driver_write(datetime);

            status = (status == OKAY) ? OKAY : I2C_ERROR;
        }
        else
        {
            // send ntp error message to the teensy
            status = i2c_driver_write((uint8_t *)NTP_ERROR);
            status = (status == OKAY) ? OKAY : I2C_ERROR;
        }
    }
    else
    {
        // send wifi disconnected message to the teensy
        i2c_driver_write((uint8_t *)WIFI_DISCONNECTED);
    }
}
