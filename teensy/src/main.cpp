#include <bsp.h>
#include <canbus.h>
#include <common.h>
#include <bsp_time.h>

void setup()
{
#ifdef DEVELOPMENT
    bsp_serial_begin();
#endif

    bsp_delay(3000);
    PRINTF("%d\n", get_humidity());
}

void loop()
{
}