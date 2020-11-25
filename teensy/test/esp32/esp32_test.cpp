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

static uint8_t esp32_status;
static uint8_t rtc_status;
static uint8_t rtc_year;
static uint8_t rtc_month;
static uint8_t rtc_day;
static uint8_t rtc_hour;
static uint8_t rtc_minute;
static uint8_t rtc_second;

static void reset_variables()
{
    esp32_status = 0;
    rtc_status = 0;
}
//----------------------------------- functions -----------------------------------------------//

uint16_t bsp_year(void)
{
}
uint8_t bsp_month(void)
{
}
uint8_t bsp_day(void)
{
}
uint8_t bsp_hour(void)
{
}
uint8_t bsp_minute(void)
{
}
uint8_t bsp_second(void)
{
}
void bsp_delay(uint32_t ms)
{
}
void bsp_set_time(uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, uint8_t _second)
{
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
    uint8_t status = ERROR;

    if (request && recieve)
    {

#ifdef TARGET
        // Wire.begin(I2C_SLAVE, ADDRESS, I2C_PINS_18_19, I2C_PULLUP_INT, CLOCK_RATE);
        //Wire.onReceive(recieve);
        // Wire.onRequest(request);

#endif
        status = OKAY;
    }

    return status;
}
uint8_t i2c_driver_read(uint8_t *data, size_t size)
{
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
    TEST_ASSERT_EQUAL_UINT8(OKAY, esp32_status);
}
void test_rtc_status(void)
{
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

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}