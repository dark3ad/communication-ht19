#include <bsp.h>
#include <stdlib.h>
#include <string.h>
#include <bsp_time.h>
#include <scheduler.h>

#define MAX_NAME_LENGTH (16U)

typedef struct
{
    func_t run;
    func_t init;
    uint8_t priority;
    uint16_t interval;
    uint32_t last_run_ms;
    char name[MAX_NAME_LENGTH + 1];
} task_t;

static uint8_t counter = 0U;
static task_t tasks[MAX_TASKS] = {};
static volatile uint32_t ms_counter = 0U;

static void timer_run(void)
{
    noInterrupts();
    ms_counter++; // Increase the ms counter. Allow it to overflow.
    interrupts();
}

void scheduler_init(func_t idle_init, func_t idle_run)
{
    bsp_timer_init(timer_run);

    for (uint8_t i = 0; i < MAX_TASKS; i++)
    {
        tasks[i].run = NULL;
        tasks[i].init = NULL;
        tasks[i].interval = 0U;
        tasks[i].priority = 0xFFU;
        tasks[i].last_run_ms = 0U;
        memset(tasks[i].name, 0, MAX_NAME_LENGTH + 1);
    }

    if (idle_run != NULL)
    {
        counter++;
        tasks[0].run = idle_run;
        tasks[0].init = idle_init;
        tasks[0].priority = 0xFEU;
        strncpy(tasks[0].name, "Idle Task", MAX_NAME_LENGTH);
    }
}

bool scheduler_add_task(const char *name, func_t init, func_t run, uint8_t priority, uint16_t interval)
{
    bool status = false;

    if ((counter < MAX_TASKS) && (name != NULL) && (run != NULL) && (priority < 0xFEU) && (interval != 0U))
    {
        for (uint8_t i = 0U; i < MAX_TASKS; i++)
        {
            if (tasks[i].priority > priority)
            {
                if (i < MAX_TASKS - 1)
                {
                    memmove(&tasks[i + 1], &tasks[i], (MAX_TASKS - i - 1) * sizeof(task_t));
                    tasks[i].run = NULL;
                }

                if (tasks[i].run == NULL)
                {
                    counter++;
                    tasks[i].run = run;
                    tasks[i].init = init;
                    tasks[i].interval = interval;
                    tasks[i].priority = priority;
                    strncpy(tasks[i].name, name, MAX_NAME_LENGTH);

                    status = true;
                    break;
                }
            }
        }
    }

    return status;
}

bool scheduler_run(void)
{
    if (bsp_timer_start())
    {
        for (uint8_t i = 0U; i < MAX_TASKS; i++)
        {
            if (tasks[i].init != NULL)
            {
                tasks[i].init();
                PRINTF("%s initialization was run\n", tasks[i].name);
            }
        }

        while (true)
        {
            for (uint8_t i = 0; i < MAX_TASKS; i++)
            {
                if (tasks[i].run != NULL)
                {
                    uint32_t last_run = 0U;

                    if (tasks[i].last_run_ms > ms_counter) // If the ms counter has been overflowed
                    {
                        last_run = (0xFFFFFFFFU - tasks[i].last_run_ms) + ms_counter + 1U;
                    }
                    else
                    {
                        last_run = ms_counter - tasks[i].last_run_ms;
                    }

                    if (last_run >= tasks[i].interval)
                    {
                        tasks[i].run();
                        tasks[i].last_run_ms = ms_counter;
                        // PRINTF("%s was run @%d\n", tasks[i].name, ms_counter);
                    }
                }
            }
        }
    }

    return false;
}