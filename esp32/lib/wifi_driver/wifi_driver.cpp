#include <WiFi.h>
#include <wifi_driver.h>

uint8_t wifi_driver_init(char *ssid, char *passwrd)
{
    uint8_t status = ERROR;

    if (ssid && passwrd)
    {
        if (WL_CONNECTED == WiFi.begin(ssid, passwrd))
        {
            status = OKEY;
        }
    }

    return status;
}

uint8_t wifi_driver_status(void)
{
    uint8_t status = ERROR;

    if (WiFi.status() == WL_CONNECTED)
    {
        status = OKEY;
    }

    return status;
}
