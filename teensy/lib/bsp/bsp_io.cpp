#include <bsp.h>
#include <bsp_io.h>

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
    pinMode(pin, mode);
}

void bsp_digital_write(uint8_t pin, uint8_t val)
{
    digitalWrite(pin, val);
}
