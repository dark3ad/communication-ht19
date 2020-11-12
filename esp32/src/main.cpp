#include <bsp.h>
#include <ntp.h>

void setup()
{
    bsp_serial_begin();
    ntp_init();
}

void loop()
{
}