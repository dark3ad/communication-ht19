#include <bsp.h>

#define BAUD_RATE (9600U)

void bsp_serial_begin(void)
{
    Serial.begin(BAUD_RATE);
    while (!Serial)
    {
    }
}