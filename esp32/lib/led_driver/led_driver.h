#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief This function is used to initialized the builtin LED.
 * 
 * @return true if initialization is okay
 * @return false if initialization is not okay
 */
bool led_driver_init(void);

/**
 * @brief This function is to turn the led on
 * 
 * @return true if the LED is on
 * @return false if the LED is not on
 */
bool led_driver_turn_on(void);

/**
 * @brief This function is to turn the led off
 * 
 * @return true if the LED is off
 * @return false if the LED is not off
 */
bool led_driver_turn_off(void);

#endif /* LED_DRIVER_H */