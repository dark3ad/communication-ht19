#ifdef TARGET
#include <Arduino.h> // In link-time we need to use Arduino on Teensy
#endif

#include <unity.h>
#include <bsp_io.h>
#include <led_driver.h>

static int16_t state;  // this indicated led on/off
static uint8_t outpin; // pin in OUTPUT mode

void reset_variables()
{
    state = 0;
    outpin = 0;
}

void bsp_digital_write(uint8_t pin, uint8_t val)
{

    // set state to 1 if val is high and set state to -1 if val is low
    state = (val == HIGH) ? 1 : -1;

#ifdef TARGET
    digitalWrite(pin, val); // digitalWrite of Arduino on Teensy
#endif
}

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT)
    {
        // If the mode is correct, increament outpins
        outpin++;
    }
#ifdef TARGET
    pinMode(pin, mode); // pinMode of Arduino on Teensy
#endif
}

uint8_t bsp_digital_read(uint8_t pin)
{

#ifdef TARGET
    digitalRead(pin);
#endif

    // return HIGH if state is 1  and LOW if state is  0 or -1
    return (state == 1) ? HIGH : LOW;
}

void setUp()
{
    reset_variables();
}

void tearDown()
{
}

void test_led_driver_init(void)
{
    /* Test for set output pinmode */
    TEST_ASSERT_EQUAL_UINT8(0, outpin);
    TEST_ASSERT_EQUAL_UINT8(OKAY, led_driver_init());
    TEST_ASSERT_EQUAL_UINT8(1, outpin);

    /* Test for  state when the driver begins */
    TEST_ASSERT_EQUAL_UINT8(-1, state);
}

void test_led_driver_turn_on()
{
    /* Test for led state on */
    TEST_ASSERT_EQUAL_UINT8(OKAY, led_driver_init());
    TEST_ASSERT_EQUAL_UINT8(-1, state);
    TEST_ASSERT_EQUAL_UINT8(OKAY, led_driver_turn_on());
    TEST_ASSERT_EQUAL_UINT8(1, state);
#ifdef TARGET
    delay(1000);
#endif
}

void test_led_driver_turn_off()
{
    /* Test for led state off */
    TEST_ASSERT_EQUAL_UINT8(OKAY, led_driver_init());
    TEST_ASSERT_EQUAL_UINT8(-1, state);
    TEST_ASSERT_EQUAL_UINT8(OKAY, led_driver_turn_off());
    TEST_ASSERT_EQUAL_UINT8(-1, state);
#ifdef TARGET
    delay(1000);
#endif
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    delay(3000);
#else
int main()
{
#endif
    UNITY_BEGIN();
    RUN_TEST(test_led_driver_init);
    RUN_TEST(test_led_driver_turn_on);
    RUN_TEST(test_led_driver_turn_off);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}