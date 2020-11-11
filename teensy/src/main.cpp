#include <bsp.h>
#include <esp32.h>

void setup()
{
    bsp_serial_begin();
    esp32_init();
}

void loop()
{
    esp32_run();
}