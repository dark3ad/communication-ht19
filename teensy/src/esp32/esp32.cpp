#include <stdio.h>
#include <esp32.h>
#include <canbus.h>
#include <string.h>
#include <common.h>
#include <candata.h>
#include <bsp_time.h>
#include <i2c_driver.h>

#define DATETIME_LENGTH (19U)

static void handle_request(void);
static void handle_receive(size_t length);

void esp32_init(void)
{
    uint8_t status = i2c_driver_init(handle_request, handle_receive);
    status = (status == OKAY) ? OKAY : I2C_ERROR;
    set_esp32_status(status);
}

void esp32_run(void)
{
    if (OKAY == get_rtc_status())
    {
        set_rtc_year(bsp_year());
        set_rtc_month(bsp_month());
        set_rtc_day(bsp_day());
        set_rtc_hour(bsp_hour());
        set_rtc_minute(bsp_minute());
        set_rtc_second(bsp_second());
    }
}

static void handle_receive(size_t length)
{
    char data[length + 1] = {};
    uint8_t esp32_status = I2C_ERROR;

    if (OKAY == i2c_driver_read((uint8_t *)data, length))
    {
        if (length == DATETIME_LENGTH)
        {
            int _year = 0, _month = 0, _minute = 0, _second = 0, _hour = 0, _day = 0;
            sscanf(data, "%04d-%02d-%02d %02d:%02d:%02d", &_year, &_month, &_day, &_hour, &_minute, &_second);
            if ((_year >= 2020) && (_year < 2048) && (_month >= 1) && (_month <= 12) && (_day >= 1) && (_day <= 31))
            {
                if ((_hour >= 0) && (_hour < 24) && (_minute >= 0) && (_minute < 60) && (_second >= 0) && (_second < 60))
                {
                    // set the RTC datetime
                    bsp_set_time(_year, _month, _day, _hour, _minute, _second);
                    set_rtc_status(OKAY);
                    esp32_status = OKAY;
                }
            }
        }
        else if (length == 1U)
        {
            switch (data[0])
            {
            case OKAY:
                esp32_status = OKAY;
                break;
            case NTP_ERROR:
                esp32_status = NTP_ERROR;
                break;
            case WIFI_DISCONNECTED:
                esp32_status = WIFI_DISCONNECTED;
                break;
            case MQTT_DISCONNECTED:
                esp32_status = MQTT_DISCONNECTED;
                break;
            case MQTT_PUBLISH_ERROR:
                esp32_status = MQTT_PUBLISH_ERROR;
                break;
            default:
                esp32_status = UNINITIALIZED;
                break;
            }
        }
    }

    set_esp32_status(esp32_status);
}

static void handle_request(void)
{
    char buffer[PAYLOADS_LENGTH] = {};
    get_payloads(buffer);

    uint8_t status = i2c_driver_write((uint8_t *)buffer, strlen(buffer));
    set_esp32_status((status == OKAY) ? OKAY : I2C_ERROR);
}
