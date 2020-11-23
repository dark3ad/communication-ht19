#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

#ifndef TARGET
#endif

uint8_t bsp_serial_available(void);

void bsp_serial_write(uint8_t * buf, uint16_t len);

void bsp_serial_read(char * buffer);

#endif
