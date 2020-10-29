
#include <Wire.h>
#include <Arduino.h>
#include <i2c_driver.h>

#define SLAVE 0x00
#define CLOCK_RATE 400000

uint8_t i2c_driver_init(void)
{
    uint8_t status = ERROR;

    if (Wire.begin(SDA, SCL, CLOCK_RATE))
    {
        Wire.setTimeOut(2000);
        status = OKAY;
    }

    return status;
}

uint8_t i2c_driver_write(uint8_t *data)
{

    uint8_t status = ERROR;

    if (data != NULL)
    {
        size_t size = strlen((char *)data) + 1;

        Wire.beginTransmission(SLAVE);

        if (size == Wire.write(data, size))
        {
            // If true, endTransmission() sends a stop message after transmission, releasing the I2C bus.
            // If false, endTransmission() sends a restart message after transmission.
            // It returns last error

            Wire.endTransmission(true);

            //if it is  0 -> no error and any number other than 0 -> error
            if (!Wire.lastError())
            {
                status = OKAY;
            }
        }
    }
    return status;
}

uint8_t i2c_driver_read(uint8_t *data, uint16_t length)
{
    uint8_t status = ERROR;

    // It returns last error
    Wire.readTransmission(SLAVE, data, length, true);

    if (!Wire.lastError() && length == strlen((char *)data) + 1)
    {
        status = OKAY;
    }

    return status;
}
