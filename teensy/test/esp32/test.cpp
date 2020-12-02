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

extern void handle_request(void);
extern void handle_receive(size_t length);

static uint8_t i2c_status = 0;

uint8_t i2c_driver_init(void (*request)(void), void (*recieve)(size_t))
{
    return i2c_status;
}

uint8_t i2c_driver_read(uint8_t *data, size_t size)
{
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
}

void tearDown(void)
{
}

void test_esp32_init(void)
{
    /* i2c_status = OKAY;
    esp32_init();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_esp32_status());

    i2c_status = ERROR;
    esp32_init();
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_esp32_status());*/
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

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}