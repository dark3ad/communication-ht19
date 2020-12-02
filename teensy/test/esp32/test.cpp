#ifdef TARGET
#include <Arduino.h>
#endif

#include <unity.h>
#include <esp32.h>
#include <common.h>
#include <canbus.h>
#include <candata.h>
#include <bsp_time.h>
#include <i2c_driver.h>

#include <string.h>

extern void handle_request(void);
extern void handle_receive(size_t length);

// Assume that data received from esp32 via i2c
static char rx_buffer[19 + 1] = {};
// faking variables
static uint8_t i2c_status;
static uint16_t Year;
static uint8_t Month;
static uint8_t Day;
static uint8_t Hour;
static uint8_t Minute;
static uint8_t Second;

static void reset_variables()
{
    i2c_status = UNINITIALIZED;
    Year = 0;
    Month = 0;
    Day = 0;
    Hour = 0;
    Minute = 0;
    Second = 0;
    memset(rx_buffer, 0, sizeof(rx_buffer));
}
//-------------------------------- faked functions --------------------------------//
uint8_t i2c_driver_init(void (*request)(void), void (*recieve)(size_t))
{
    return i2c_status;
}

uint8_t i2c_driver_read(uint8_t *data, size_t size)
{
    memcpy(data, rx_buffer, size);
    i2c_status = OKAY;
    return i2c_status;
}

uint8_t i2c_driver_write(uint8_t *data, size_t size)
{
    return i2c_status;
}

uint16_t bsp_year(void)
{
    return Year;
}

uint8_t bsp_month(void)
{
    return Month;
}

uint8_t bsp_day(void)
{
    return Day;
}

uint8_t bsp_hour(void)
{
    return Hour;
}

uint8_t bsp_minute(void)
{
    return Minute;
}

uint8_t bsp_second(void)
{
    return Second;
}

void bsp_delay(uint32_t ms)
{
}

void bsp_set_time(uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, uint8_t _second)
{
    Year = _year;
    Month = _month;
    Day = _day;
    Hour = _hour;
    Minute = _minute;
    Second = _second;
}
//--------------------------------------Testing -----------------------------------//
void setUp(void)
{
    reset_variables();
}

void tearDown(void)
{
}

// checking esp32 status after it initialized
void test_esp32_init(void)
{
    i2c_status = OKAY;
    esp32_init();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_esp32_status());

    i2c_status = ERROR;
    esp32_init();
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_esp32_status());
}
// By assuming rx_buffer is date_time from esp32 [ YYYY-MM-DD HH:MM:SS ] via 12c
// checking rtc status
void test_rtc_status(void)
{
    int size;

    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_rtc_status());

    // if time length is not  equal to 19 and  time formate is not correct
    // expecting RTC status still uninitialized
    sprintf(rx_buffer, "%d-%d-%d %d:%d", 2049, 11, 60, 11, 20);
    size = strlen((char *)rx_buffer);
    handle_receive(size);
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_rtc_status());

    // if time length is  equal to 19 and  time formate is also correct
    // expecting RTC status Okay
    sprintf(rx_buffer, "%d-%d-%d %d:%d:%d", 2020, 11, 25, 11, 20, 30);
    size = strlen((char *)rx_buffer);
    // handle_receive(size);
    // TEST_ASSERT_EQUAL_UINT8(OKAY, get_rtc_status());
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
int main(void)
{
#endif
    UNITY_BEGIN();

    RUN_TEST(test_esp32_init);
    RUN_TEST(test_rtc_status);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}