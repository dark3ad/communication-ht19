#ifndef BSP_H
#define BSP_H

#include <common.h>

#ifdef TARGET
#include <Arduino.h>
#ifdef DEVELOPMENT
#define PRINTF(frmt, ...) Serial.printf(frmt, __VA_ARGS__)
#else
#define PRINTF(frmt, ...)
#endif

void bsp_serial_begin(void);

#else
#include <stdio.h>

#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3

#ifdef DEVELOPMENT
#define PRINTF(frmt, ...) printf(frmt, __VA_ARGS__)
#else
#define PRINTF(frmt, ...)
#endif
#endif

#endif /* BSP_H */
