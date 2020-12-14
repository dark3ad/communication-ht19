
#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <ctype.h>
#include <stdint.h>

/**
 * @brief This function is used to initialize the i2c_driver for slave mode(Teensy)
 * 
 * @param request is a function pointer to handle request. 
 * @param recieve is a function pointer to handle receieve. 
 *                where size_t is size of the data to receive.
 * @return uint8_t  status of the function. It can be ERROR or OKAY 
 */
uint8_t i2c_driver_init(void (*request)(void), void (*recieve)(size_t));

/**
 * @brief This function is used to write data to the master
 * 
 * @param data which is suppossed to be write to the master
 * @param size size of the data (number of bytes)
 * @return uint8_t status of the function. It can be  OKAY or ERROR
 */
uint8_t i2c_driver_write(uint8_t *data, size_t size);

/**
 * @brief This function is used to read data from the master
 * 
 * @param data which is suppossed to be read from the master
 * @param size size of the data (number of bytes)
 * @return uint8_t status of the function. It can be  OKAY or ERROR
 */
uint8_t i2c_driver_read(uint8_t *data, size_t size);

#endif /* I2C_DRIVER_H */