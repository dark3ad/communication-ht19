/**
 * @file wifi_driver.h
 * @author Bengt Cruz (bengt.cruz@ya.se)
 * @brief Wifi driver intended for ESP32 in GreenHouse project
 * @version 0.1
 * @date 2020-11-04
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef WIFI_DRIVER_H
#define WIFI_DRIVER_H

#include <WiFi.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief This function is used to initialize the WiFi driver
 * 
 * @param ssid The WiFi SSID
 * @param password The WiFi password
 */
void wifi_driver_init(const char *ssid, const char *password);

/**
 * @brief This function is used to connect to Internet.
 * 
 */
void wifi_driver_connect(void);

/**
 * @brief This function is used to check the if there is a connection or not
 * 
 * @return true if the module is connected to Internet
 * @return false  if the module is not connected to Internet
 */
bool wifi_driver_status(void);

#endif /* WIFI_DRIVER_H */
