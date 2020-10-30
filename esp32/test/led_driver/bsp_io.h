#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

#ifndef LOW
#define LOW 0
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef INPUT
#define INPUT 0
#endif

#ifndef OUTPUT
#define OUTPUT 1
#endif

#ifndef INPUT_PULLUP
#define INPUT_PULLUP 2
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

void bsp_pin_mode(uint8_t pin, uint8_t mode);

void bsp_digital_write(uint8_t pin, uint8_t val);

uint8_t bsp_digital_read(uint8_t pin);

#endif