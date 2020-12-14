/**
 * @file i2c_driver.h
 * @author PraveenaSiva (pravisiva757@gmail.com)
 * @brief  I2C driver is used for the application in master mode on esp32.
 * @version 0.1
 * @date 2020-10-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief This function is used to initialize the i2c_driver for master mode on esp32
 * 
 * @return true if initialization is okay
 * @return false if initialization is not okay
 */
bool i2c_driver_init(void);

/**
 * @brief This function is used to write data to the slave(teensy)
 * 
 * @param data which is suppossed to be write to the slave(teensy)
 * @param size which is size of the data
 * @return true if i2c write is succeeded
 * @return false if i2c write is not succeeded
 */
bool i2c_driver_write(uint8_t *data, size_t size);

/**
 * @brief This function is used to read data from the slave(teensy)
 * 
 * @param data which is suppossed to be read from the slave(teensy)
 * @param size which is size of the data
 * @return true if i2c read is succeeded
 * @return false if i2c read is not succeeded
 */
bool i2c_driver_read(uint8_t *data, size_t size);

#endif /* I2C_DRIVER_H */