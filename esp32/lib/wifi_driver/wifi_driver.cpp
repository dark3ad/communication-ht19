#include <wifi_driver.h>

void wifi_driver_init(const char *ssid, const char *password)
{
    WiFi.begin(ssid, password);
}

bool wifi_driver_status(void)
{
    return (WiFi.status() != WL_CONNECTED);
}

void wifi_driver_connect(void)
{
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
}
