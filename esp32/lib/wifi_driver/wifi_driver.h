#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>

/**
 * @brief driver to initialize the wifi connection on the esp32
 * 
 * @param ssid wifi ssid 
 * @param passwrd password to the wifi
 */
void wifi_driver_init(char *ssid, char *passwrd);

/**
 * @brief Returns the status of the WiFi connection... 
 * 
 */

void wifi_status_driver(void);

#endif // !WIFI_H
