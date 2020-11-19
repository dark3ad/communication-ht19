
#include <Wire.h>
#include <Arduino.h>
#include <i2c_driver.h>

#define SLAVE 0x00
#define CLOCK_RATE 400000

bool i2c_driver_init(void)
{
    uint8_t status = false;

    if (Wire.begin(SDA, SCL, CLOCK_RATE))
    {
        Wire.setTimeOut(2000);
        status = true;
    }

    return status;
}

bool i2c_driver_write(uint8_t *data, size_t size)
{
    uint8_t status = false;

    if (data != NULL)
    {
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
                status = true;
            }
        }
    }
    return status;
}

bool i2c_driver_read(uint8_t *data, size_t size)
{
    bool status = false;

    // It returns last error
    Wire.readTransmission(SLAVE, data, size, true);

    if (!Wire.lastError())
    {
        status = true;
    }

    return status;
}
