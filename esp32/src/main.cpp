/**
 * @file main.cpp
 * @author 
 * @brief MQTT and NTP Server Application to GreenHouse system.
 * @version 0.1
 * @date 2020-11-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <Arduino.h>
#include <wifi_driver.h>
#include <i2c_driver.h>
#include <led_driver.h>
#include "time.h"

#define SSID "MOVISTAR_11E9"
#define PASS "TSZkcb5rycfKGfAsdNf3"

void printLocalTime();

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    // Connect to Wi-Fi
    Serial.print("Connecting to ");
    Serial.println(SSID);
    wifi_driver_init(SSID, PASS);
    while (wifi_driver_status() != CONNECTED)
    {
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected.");

    // Init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();
}

void loop()
{
    delay(1000);
    printLocalTime();
}

void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    Serial.println();
}
