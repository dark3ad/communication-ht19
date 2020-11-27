#ifdef TARGET
#include <Arduino.h> // In link-time we need to use Arduino on Teensy
#endif

#include <unity.h>
#include <bsp_time.h>
#include <common.h>
#include <canbus.h>
#include <candata.h>
#include <i2c_driver.h>
#include <esp32.h>

#include <stdio.h>
#include <string.h>

#define DATETIME_LENGTH (19U)

static uint8_t esp32_status;
static uint8_t i2c_status;
static uint8_t rtc_status;

static uint16_t rtc_year;
static uint8_t rtc_month;
static uint8_t rtc_day;
static uint8_t rtc_hour;
static uint8_t rtc_minute;
static uint8_t rtc_second;

static uint16_t year;
static uint8_t month;
static uint8_t day;
static uint8_t hour;
static uint8_t minute;
static uint8_t second;

static uint16_t rx_buffer[20] = {};
static uint8_t payload_buffer[50] = {}; // just try

static void reset_variables()
{
    esp32_status = 0;
    rtc_status = 0;
    i2c_status = 0;
    year = 0;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
    second = 0;
    rtc_year = 0;
    rtc_month = 0;
    rtc_day = 0;
    rtc_hour = 0;
    rtc_minute = 0;
    rtc_second = 0;
}

static void handle_receive(size_t length)
{
    char data[length + 1] = {};

    esp32_status = I2C_ERROR;

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
    /*char buffer[PAYLOADS_LENGTH] = {};
    get_payloads(buffer);

    uint8_t status = i2c_driver_write((uint8_t *)buffer, strlen(buffer));
    set_esp32_status((status == OKAY) ? OKAY : I2C_ERROR);*/
}
static void time_handling(char *time)
{
    int size = strlen(time);
    memcpy(rx_buffer, time, size);
    handle_receive(size);
}
//----------------------------------- Fake functions -----------------------------------------------//

uint16_t bsp_year(void)
{
    return year;
}
uint8_t bsp_month(void)
{
    return month;
}
uint8_t bsp_day(void)
{
    return day;
}
uint8_t bsp_hour(void)
{
    return hour;
}
uint8_t bsp_minute(void)
{
    return minute;
}
uint8_t bsp_second(void)
{
    return second;
}
void bsp_delay(uint32_t ms)
{
}
void bsp_set_time(uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, uint8_t _second)
{
    year = _year;
    month = _month;
    day = _day;
    hour = _hour;
    minute = _minute;
    second = _second;
}

uint8_t get_rtc_status(void)
{
    return rtc_status;
}
bool set_esp32_status(uint8_t value)
{
    esp32_status = value;
}
bool set_rtc_status(uint8_t value)
{
    rtc_status = value;
}
bool set_rtc_year(uint16_t value)
{
    rtc_year = value;
}
bool set_rtc_day(uint8_t value)
{
    rtc_day = value;
}
bool set_rtc_month(uint8_t value)
{
    rtc_month = value;
}
bool set_rtc_hour(uint8_t value)
{
    rtc_hour = value;
}
bool set_rtc_minute(uint8_t value)
{
    rtc_minute = value;
}
bool set_rtc_second(uint8_t value)
{
    rtc_second = value;
}
void get_payloads(char *payloads)
{
}
uint8_t i2c_driver_init(void (*request)(void), void (*recieve)(size_t))
{
    i2c_status = ERROR;

    if (request && recieve)
    {
        i2c_status = OKAY;
    }

    return i2c_status;
}
uint8_t i2c_driver_read(uint8_t *data, size_t size)
{
    i2c_status = ERROR;
    if (rx_buffer)
    {
        memcpy(data, rx_buffer, size);
        i2c_status = OKAY;
    }
    return i2c_status;
}
uint8_t i2c_driver_write(uint8_t *data, size_t size)
{
}

//clear &&rm - rf build &&mkdir - p build
//---------------------------------------------------------------------------------------------//

// Running before every "RUN_TEST(...)"

void setUp()
{
    reset_variables();
}

// Running after every "RUN_TEST(...)"
void tearDown()
{
}

void test_esp32_init(void)
{

    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, esp32_status);
    esp32_init();
    TEST_ASSERT_EQUAL_UINT8(OKAY, i2c_status);
    TEST_ASSERT_EQUAL_UINT8(OKAY, esp32_status);
}

// rx_buffer is date_time from esp32 [ YYYY-MM-DD HH:MM:SS ] via 12c
void test_rtc_status(void)
{
    esp32_init();
    TEST_ASSERT_EQUAL_UINT8(OKAY, esp32_status);

    // if  time is null, no need to handle receive(esp32 to teensy)
    char time[20] = {};
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, rtc_status);
    TEST_ASSERT_EQUAL_UINT8(OKAY, esp32_status);

    // if  time is not null but time length is not equal to 20
    sprintf(time, "%d-%d-%d", 2020, 11, 25);
    time_handling(time);

    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, rtc_status);
    TEST_ASSERT_EQUAL_UINT8(I2C_ERROR, esp32_status);

    // if time length is  equal to 19 but time formate is not correct
    sprintf(time, "%d-%d-%d-%d-%d-%d", 2020, 11, 25, 11, 20, 30);
    time_handling(time);
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, rtc_status);
    TEST_ASSERT_EQUAL_UINT8(I2C_ERROR, esp32_status);

    // if time length is  equal to 19 and  time formate is also correct
    sprintf(time, "%d-%d-%d %d:%d:%d", 2020, 11, 25, 11, 20, 30);
    time_handling(time);
    TEST_ASSERT_EQUAL_UINT8(OKAY, rtc_status);
    TEST_ASSERT_EQUAL_UINT8(OKAY, esp32_status);
}
// rx_buffer is status(WIFI_DISCONNECCTED, MQTT_DISCONNECCTED,MQTT_PUBLISH _ERROR,NTP_ERROR) from esp32 via i2c
void test_esp32_status(void)
{
    esp32_init();
    TEST_ASSERT_EQUAL_UINT8(OKAY, esp32_status);

    char error_status = {};
}

void test_ep32_run(void)
{
    char time[20] = {};

    //if rtc_status is not okay
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, rtc_status);
    esp32_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, year);
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, rtc_year);

    //if rtc_status is  okay,then  all rtc values set and we can expect the same
    sprintf(time, "%d-%d-%d %d:%d:%d", 2020, 11, 25, 11, 20, 30);
    time_handling(time);
    TEST_ASSERT_EQUAL_UINT8(OKAY, rtc_status);
    esp32_run();
    TEST_ASSERT_EQUAL_UINT8(2020, rtc_year);
    TEST_ASSERT_EQUAL_UINT8(11, rtc_month);
    TEST_ASSERT_EQUAL_UINT8(25, rtc_day);
    TEST_ASSERT_EQUAL_UINT8(11, rtc_hour);
    TEST_ASSERT_EQUAL_UINT8(20, rtc_minute);
    TEST_ASSERT_EQUAL_UINT8(30, rtc_second);
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    delay(3000); // We need to have a delay because the tempory file
                 // that is being created in a test run doesn't include a delay.
#else
int main()
{
#endif
    UNITY_BEGIN();

    RUN_TEST(test_esp32_init);
    RUN_TEST(test_rtc_status);
    RUN_TEST(test_ep32_run);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}