#include <ntp.h>
#include <common.h>
#include <wifi_driver.h>
#include <i2c_driver.h>
#include "time.h"

#define TIME_OFFSET (1 * 60 * 60)
#define TIME_DAYLIGHT (1 * 60 * 60)
#define NTP_ADDRESS "pool.ntp.org"

void ntp_datetime_init(void)
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

void get_ntp_datetime(char *ntp)
{
    uint8_t status = NTP_ERROR;

    struct tm timeinfo;

    if (getLocalTime(&timeinfo))
    {
        strftime(ntp, DATE_TIME_LENGTH, "%Y-%m-%d %H:%M:%S", &timeinfo);
        status = OKAY;
    }

    // set ntp status  ??
}

void send_ntp_datetime(uint8_t *datetime)
{

    uint8_t status = i2c_driver_write(datetime);
    status = (status == OKAY) ? OKAY : I2C_ERROR;

    // set_esp32_status(status);
}