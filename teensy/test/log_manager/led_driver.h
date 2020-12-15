/**
 * @file led_driver.h
 * @author PraveenaSiva (pravisiva757@gmail.com)
 * @brief  This led driver is mainly for the indication of error. If there is an critical error when storing files
 *         in the log files, shall led turn on to indicate that there is an error.
 * @version 0.1
 * @date 2020-10-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>

#ifndef LOW
#define LOW 0
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef OUTPUT
#define OUTPUT 1
#endif

/**
 * @brief This function is used to initialize the led driver
 *         Set the pinmode output and set built_in_led off in the beginning.
 * @return uint8_t the status. It can be ERROR or OKAY
 */
uint8_t led_driver_init(void);

/**
 * @brief This function is to turn the led on
 * 
 * @return uint8_t the status. It can be ERROR or OKAY
 */
uint8_t led_driver_turn_on(void);

/**
 * @brief This function is to turn the led off
 * 
 * @return uint8_t the status. It can be ERROR or OKAY
 */
uint8_t led_driver_turn_off(void);

#endif /* LED_DRIVER_H */