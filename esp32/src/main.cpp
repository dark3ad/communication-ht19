/**
 * @file main.cpp
 * @author PraveenaSiva (pravisiva757@gmail.com)
 * @brief Aplication for NTP and MQTT 
 *        NTP -> To initialize RTC on Teensy , MQTT -> To publish Topics(signals from CAN bus ) 
* @version 0.1
 * @date 2020-11-23
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <bsp.h>
#include <time.h>
#include <common.h>
#include <bsp_time.h>
#include <i2c_driver.h>
#include <led_driver.h>
#include <wifi_driver.h>
#include <MQTTClient.h>

// should include <wifi.h> in <wifi_driver.h> for mqtt
#include <wifi.h>

#define SSID "YA-OPEN"
#define PASS "utbildning2015"
#define DATE_TIME_LENGTH (19U)
#define MAX_SIGNAL_SIZE (3U) // message size (2 byte + 1)

WiFiClient wifi;
MQTTClient client;

void setup()
{
#ifdef DEVELOPMENT
    bsp_serial_begin();
#endif

    led_driver_init();

    if (!i2c_driver_init())
    {
        while (true)
        {
            // blink the led
            led_driver_turn_on();
            bsp_delay(500);
            led_driver_turn_off();
            bsp_delay(500);
        }
    }

    wifi_driver_init(SSID, PASS);
    wifi_driver_connect();

    client.begin(SERVER, PORT, wifi);

    //NTP init and get the time
    configTime(TIME_OFFSET, TIME_DAYLIGHT, NTP_ADDRESS);

    uint8_t status = NTP_ERROR;

    while (status != OKAY)
    {
        struct tm timeinfo;
        uint8_t datetime[DATE_TIME_LENGTH + 1] = {};

        // get time
        if (getLocalTime(&timeinfo))
        {
            strftime((char *)datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", &timeinfo);

            // write date time to teensy and set the status to okay
            if (i2c_driver_write(datetime, DATE_TIME_LENGTH))
            {
                status = OKAY;
            }
        }
        else
        {
            // Send ntp error message to the teensy
            i2c_driver_write(&status, sizeof(status));
        }

        bsp_delay(1000);
    }
}

void loop()
{

    client.loop();
    uint8_t status;

    if (!client.connected())
    {
        if (!wifi_driver_status())
        {
            status = WIFI_DISCONNECTED;

            //send WIFI_DISCONNECTED message to teensy
            i2c_driver_write(&status, sizeof(status));
            wifi_driver_connect();
        }

        while (!client.connect(CLIENT_ID, USERNAME, PASSWORD))
        {
            status = MQTT_DISCONNECTED;

            //send MQTT_DISCONNECTED message to teensy
            i2c_driver_write(&status, sizeof(status));

            Serial.print(".");
            bsp_delay(1000);
        }
    }

    char buffer[PAYLOADS_LENGTH] = {};

    // reading buffer from teensy via i2c
    if (i2c_driver_read((uint8_t *)buffer, sizeof(buffer)))
    {
        //variable to store parsed text from buffer when it finds "|"
        char message[MAX_SIGNAL_SIZE] = {};
        uint8_t topic_index = 0, i = 0;

        for (char *ptr = (char *)buffer; *ptr != 0; ptr++)
        {
            if (*ptr == '|')
            {
                message[i] = '\0';
                i = 0;

                if (strlen(message))
                {
                    if (topic_index < TOPICS_NUMBER)
                    {
                        //client.publish("/Topic", value);
                        if (client.publish(topics[topic_index], message))
                        {
                            topic_index++;
                        }
                        else
                        {
                            status = MQTT_PUBLISH_ERROR;
                            //send MQTT_PUBLISH_ERROR message to teensy
                            i2c_driver_write(&status, sizeof(status));
                        }
                    }
                }
            }
            else
            {
                message[i] = *ptr;
                i++;
            }
        }
    }

    // should publish every 5  sec
    bsp_delay(5000);
}