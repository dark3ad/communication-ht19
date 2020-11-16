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

    // do we need to send the status to teensy??
    // if 12c itself is a problem, then how can we send the message via i2c??

    //connect to WiFi
    wifi_driver_init();
    wifi_driver_connect();

    //NTP init and get the time
    configTime(TIME_OFFSET, TIME_DAYLIGHT, NTP_ADDRESS);
}

void ntp_run(void)
{
    uint8_t datetime[DATE_TIME_LENGTH] = {};
    uint8_t *send_message;

    struct tm timeinfo;

    if (wifi_driver_status())
    {
        // memset(send_message, 0, sizeof(send_message));  ??

        if (getLocalTime(&timeinfo))
        {
            strftime((char *)datetime, DATE_TIME_LENGTH, "%Y-%m-%d %H:%M:%S", &timeinfo);

            // send date time to the teensy
            memcpy(send_message, datetime, sizeof(datetime));
        }
        else
        { // send ntp error message to the teensy
            memcpy(send_message, (uint8_t *)NTP_ERROR, sizeof(NTP_ERROR));
        }
    }
    else
    { // send wifi disconnected message to the teensy
        memcpy(send_message, (uint8_t *)WIFI_DISCONNECTED, sizeof(WIFI_DISCONNECTED));
    }

    // sending message to the teensy via 12c
    i2c_driver_write(send_message);
}
