#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>

#define ERROR 0
#define OKAY 1

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