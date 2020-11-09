#ifndef BSP_TIME_H
#define BSP_TIME_H

#include <stdint.h>

uint16_t bsp_year(void);
uint8_t bsp_month(void);
uint8_t bsp_day(void);
uint8_t bsp_hour(void);
uint8_t bsp_minute(void);
uint8_t bsp_second(void);
void bsp_delay(uint32_t ms);

#endif /* BSP_TIME_H */