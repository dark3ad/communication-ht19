
#include <bsp_io.h>
#include <led_driver.h>

bool led_driver_init(void)
{
    bool status = false;

    bsp_pin_mode(LED_BUILTIN, OUTPUT);

    bsp_digital_write(LED_BUILTIN, LOW);

    if (LOW == bsp_digital_read(LED_BUILTIN))
    {
        status = true;
    }

    return status;
}

bool led_driver_turn_on(void)
{
    bool status = false;
    bsp_digital_write(LED_BUILTIN, HIGH);

    if (HIGH == bsp_digital_read(LED_BUILTIN))
    {
        status = true;
    }

    return status;
}

bool led_driver_turn_off(void)
{
    bool status = false;
    bsp_digital_write(LED_BUILTIN, LOW);

    if (LOW == bsp_digital_read(LED_BUILTIN))
    {
        status = true;
    }

    return status;
}