
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

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN 3
#endif

void bsp_pin_mode(uint8_t pin, uint8_t mode);

#endif
