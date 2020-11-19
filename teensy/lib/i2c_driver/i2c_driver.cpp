#include <i2c_t3.h>
#include <common.h>
#include <i2c_driver.h>

#define ADDRESS (0x00U)
#define CLOCK_RATE (400000U) // In slave mode the clock rate is not used.

uint8_t i2c_driver_init(void (*request)(void), void (*recieve)(size_t))
{
    uint8_t status = ERROR;

    if (request && recieve)
    {
        Wire.begin(I2C_SLAVE, ADDRESS, I2C_PINS_18_19, I2C_PULLUP_INT, CLOCK_RATE);
        Wire.onReceive(recieve);
        Wire.onRequest(request);
        status = OKAY;
    }

    return status;
}

uint8_t i2c_driver_read(uint8_t *data, size_t size)
{
    uint8_t status = ERROR;

    if (data && size && (size == Wire.read(data, size)))
    {
        status = OKAY;
    }

    return status;
}

uint8_t i2c_driver_write(uint8_t *data, size_t size)
{
    uint8_t status = ERROR;

    if (data && size && (size == Wire.write(data, size)))
    {
        status = OKAY;
    }

    return status;
}