#include <bsp.h>
#include <esp32.h>
#include <canbus.h>
#include <common.h>
#include <bsp_time.h>
#include <i2c_driver.h>

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
    uint8_t RTC_status = UNINITIALIZED;
    uint8_t esp32_status = I2C_ERROR;

    char data[length + 1] = {};

    if (OKAY == get_esp32_status())
    {
        if (OKAY == i2c_driver_read((uint8_t *)data, length))
        {
            esp32_status = OKAY;

            // For date time YYYY-MM-DD HH:MM:SS
            if (length == 20U)
            {
                int _year = 0, _month = 0, _minute = 0, _second = 0, _hour = 0, _day = 0;
                sscanf(data, "%04d-%02d-%02d %02d:%02d:%02d", &_year, &_month, &_day, &_hour, &_minute, &_second);

                if (_year >= 2020 && (_month >= 1 && _month <= 12) && (_day >= 1 && _day <= 31) && (_minute > 0 && _minute < 60) && (_hour > 0 && _hour < 24) && (_second > 0 && _second < 60))
                {
                    // set the RTC datetime
                    bsp_set_time(_year, _month, _day, _hour, _minute, _second);
                    RTC_status = OKAY;
                }
            } // For the status message from esp32 to teensy
            else if (length == 1U)
            {
                switch (data[0])
                {
                case UNINITIALIZED:
                    esp32_status = UNINITIALIZED;
                    break;
                case OKAY:
                    esp32_status = OKAY;
                    break;
                case I2C_ERROR:
                    esp32_status = I2C_ERROR;
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
                    esp32_status = I2C_ERROR;
                }
            } //unknown status
            else
            {
                esp32_status = I2C_ERROR;
            }
        }
    }
    set_esp32_status(esp32_status);
    set_rtc_status(RTC_status);
}

static void handle_request(void)
{
    // yet to fill
    // Can signals send to the esp32 via i2c to make a topics in MQTT

    /* if (i2c_driver_write(tx_buffer, length))
    {
        status = OKAY;
    }*/
}
