#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

#ifndef TARGET
#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3
#define LED_BUILTIN 13
#endif

void bsp_pin_mode(uint8_t pin, uint8_t mode);

void bsp_digital_write(uint8_t pin, uint8_t val);

uint8_t bsp_digital_read(uint8_t pin);

#endif