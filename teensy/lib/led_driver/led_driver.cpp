#include <bsp_io.h>
#include <common.h>
#include <led_driver.h>

uint8_t led_driver_init(void)
{
    uint8_t status = ERROR;

    bsp_pin_mode(LED_BUILTIN, OUTPUT);

    bsp_digital_write(LED_BUILTIN, LOW);

    if (LOW == bsp_digital_read(LED_BUILTIN))
    {
        status = OKAY;
    }

    return status;
}

uint8_t led_driver_turn_on(void)
{
    uint8_t status = ERROR;
    bsp_digital_write(LED_BUILTIN, HIGH);

    if (HIGH == bsp_digital_read(LED_BUILTIN))
    {
        status = OKAY;
    }

    return status;
}

uint8_t led_driver_turn_off(void)
{
    uint8_t status = ERROR;
    bsp_digital_write(LED_BUILTIN, LOW);

    if (LOW == bsp_digital_read(LED_BUILTIN))
    {
        status = OKAY;
    }

    return status;
}