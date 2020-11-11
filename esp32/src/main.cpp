#include <bsp.h>
#include <ntp.h>

void setup()
{
    bsp_serial_begin();
    ntp_datetime_init();
}

void loop()
{
}