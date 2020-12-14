#include <bsp.h>
#include <esp32.h>
#include <common.h>
#include <bsp_time.h>
#include <scheduler.h>
#include <can_service.h>

void print_error(const char *msg)
{
    while (true)
    {
        PRINTF("%s\n", msg);
        bsp_delay(1000);
    }
}

void setup()
{
#ifdef DEVELOPMENT
    bsp_serial_begin();
    bsp_delay(5000);
#endif

    scheduler_init(NULL, NULL);

    if (!scheduler_add_task("ESP32", esp32_init, esp32_run, 0, ESP32_INTERVAL))
    {
        print_error("Failed to add ESP32 application to the scheduler");
    }

    if (!scheduler_add_task("CAN Service", can_service_init, can_service_run, 2, CAN_INTERVAL))
    {
        print_error("Failed to add CAN service to the scheduler");
    }

    if (!scheduler_run())
    {
        print_error("Failed to start the scheduler");
    }
}

void loop()
{
}