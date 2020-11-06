#include "ntp.h"
#include <wifi_driver.h>
#include <i2c_driver.h>
#include "time.h"

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

uint8_t ntp_datetime_init(void)
{
    //connect to WiFi
    Serial.printf("\n\nConnecting to %s ", SSID);
    wifi_driver_init(SSID, PASSWORD);
    while (wifi_driver_status() != CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWifi CONNECTED\n");

    //NTP init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    return i2c_driver_init();
}

uint8_t get_ntp_datetime(uint8_t *ntp)
{

    struct tm timeinfo;

    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return ERROR;
    }

    strftime((char *)ntp, DATE_TIME_LENGTH, "%Y-%m-%d %H:%M:%S", &timeinfo);

    return OKAY;
}

uint8_t send_get_ntp_datetime(uint8_t *datetime)
{

    if (!i2c_driver_write(datetime))
    {
        Serial.printf("Failed to send\n");
        return ERROR;
    }

    return OKAY;
}