#ifndef WIFI_DRIVER_H
#define WIFI_DRIVER_H

#define DISCONNECTED (0U)
#define CONNECTED (1U)

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
 * @brief This function is used to check the status of the connection
 * 
 * @return uint8_t OKEY, if there is a connection. Otherwise ERROR
 */
uint8_t wifi_driver_status(void);

#endif /* WIFI_DRIVER_H */
