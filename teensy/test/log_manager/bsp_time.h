#ifndef BSP_TIME_H
#define BSP_TIME_H

#include <stdint.h>

uint16_t bsp_year(void);
uint8_t bsp_month(void);
uint8_t bsp_day(void);
uint8_t bsp_hour(void);
uint8_t bsp_minute(void);
uint8_t bsp_second(void);
void bsp_set_time(uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, uint8_t _second);

#endif /* BSP_TIME_H */