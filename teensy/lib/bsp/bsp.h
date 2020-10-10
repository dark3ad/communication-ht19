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
#ifdef DEVELOPMENT
#define PRINTF(frmt, ...) printf(frmt, __VA_ARGS__)
#else
#define PRINTF(frmt, ...)
#endif
#endif

#endif /* BSP_H */