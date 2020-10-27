#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>

#define ERROR (0U)
#define OK (1U)

/**
 * @brief driver to initialize the wifi connection on the esp32
 * 
 * @param ssid wifi ssid 
 * @param passwrd password to the wifi
 * 
 * @return status of the initialization
 */
uint8_t wifi_driver_init(char *ssid, char *passwrd);

/**
 * @brief Returns the status of the WiFi connection... 
 * 
 */

uint8_t wifi_status_driver(void);

#endif // !WIFI_H
