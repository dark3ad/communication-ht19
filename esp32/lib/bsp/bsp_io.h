#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3

void bsp_pin_mode(uint8_t pin, uint8_t mode);

#endif