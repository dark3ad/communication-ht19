#include <wifi_driver.h>

uint8_t wifi_driver_init(char *ssid, char *passwrd)
{
    uint8_t status = ERROR;

    if (ssid && passwrd)
    {
        WiFi.begin(ssid, passwrd);
        status = OK;
    }

    return status;
}

uint8_t wifi_status()
{
    uint8_t status = OK;

    if (WiFi.status() != WL_CONNECTED)
    {
        status = ERROR;
    }

    return status;
}

