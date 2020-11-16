#include <WiFi.h>
#include <common.h>
#include <wifi_driver.h>

void wifi_driver_init(void)
{
    WiFi.begin(SSID, PASSWORD);
}

uint8_t wifi_driver_status(void)
{
    return (WiFi.status() != WL_CONNECTED) ? CONNECTED : DISCONNECTED;
}

void wifi_driver_connect(void)
{
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
}
