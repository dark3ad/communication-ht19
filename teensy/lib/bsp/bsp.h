#ifndef BSP_H
#define BSP_H

#include <common.h>

#ifdef TARGET
#include <Arduino.h>
#ifdef DEVELOPMENT
#define PRINTF(frmt, ...)             \
    Serial.printf(frmt, __VA_ARGS__); \
    Serial.flush()
#else
#define PRINTF(frmt, ...)
#endif

void bsp_serial_begin(void);

char bsp_serial_read(void);

void bsp_serial_write(const char *msg);

int bsp_serial_available(void);

bool bsp_serial_dtr(void);

#else
#include <stdio.h>
#ifdef DEVELOPMENT
#define PRINTF(frmt, ...) printf(frmt, __VA_ARGS__)
#else
#define PRINTF(frmt, ...)
#endif
#endif

#endif /* BSP_H */
