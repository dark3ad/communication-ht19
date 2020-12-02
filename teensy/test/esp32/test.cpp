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
static uint8_t i2c_status;

static void reset_variables()
{
    i2c_status = UNINITIALIZED;
    memset(rx_buffer, 0, sizeof(rx_buffer));
}

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
    return 0;
}

uint8_t bsp_month(void)
{
    return 0;
}

uint8_t bsp_day(void)
{
    return 0;
}

uint8_t bsp_hour(void)
{
    return 0;
}

uint8_t bsp_minute(void)
{
    return 0;
}

uint8_t bsp_second(void)
{
    return 0;
}

void bsp_delay(uint32_t ms)
{
}

void bsp_set_time(uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, uint8_t _second)
{
}

void setUp(void)
{
    reset_variables();
}

void tearDown(void)
{
}

void test_esp32_init(void)
{
    i2c_status = OKAY;
    esp32_init();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_esp32_status());

    i2c_status = ERROR;
    esp32_init();
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_esp32_status());
}
void test_rtc_status(void)
{
    int size;

    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_rtc_status());

    // if time length is not  equal to 19 and  time formate is not correct
    // expecting RTC status still uninitialized
    sprintf(rx_buffer, "%d-%d-%d %d:%d", 2049, 11, 60, 11, 20);
    size = strlen((char *)rx_buffer);
    handle_receive(20);
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