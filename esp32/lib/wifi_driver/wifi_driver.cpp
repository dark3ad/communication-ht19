#include <WiFi.h>
#include <wifi_driver.h>

uint8_t wifi_driver_init(char *ssid, char *passwrd)
{
    uint8_t status = DISCONNECTED;

    if (ssid && passwrd)
    {
        if (WL_CONNECTED == WiFi.begin(ssid, passwrd))
        {
            status = CONNECTED;
        }
    }

    return status;
}

uint8_t wifi_driver_status(void)
{
    uint8_t status = DISCONNECTED;

    if (WiFi.status() == WL_CONNECTED)
    {
        status = CONNECTED;
    }

    return status;
}
