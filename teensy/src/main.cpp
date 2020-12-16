#include <bsp.h>
#include <esp32.h>
#include <common.h>
#include <bsp_time.h>
#include <terminal.h>
#include <scheduler.h>
#include <log_manager.h>
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
    scheduler_init(terminal_init, terminal_run);

    if (!scheduler_add_task("Log Manager", log_manager_init, log_manager_run, 0, LOG_MANAGER_INTERVAL))
    {
        print_error("Failed to add Log Manager application to the scheduler");
    }

    if (!scheduler_add_task("ESP32", esp32_init, esp32_run, 1, ESP32_INTERVAL))
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
