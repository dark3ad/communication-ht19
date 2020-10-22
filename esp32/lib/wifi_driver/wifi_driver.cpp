#include <wifi_driver.h>

void wifi_driver_init(char *ssid, char *passwrd)
{
    WiFi.begin(ssid, passwrd);
}

void wifi_status_driver()
{
    WiFi.status();
}
