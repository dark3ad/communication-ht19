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

void wifi_status_driver()
{
    WiFi.status();
}
