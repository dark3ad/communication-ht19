#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_TASKS (4U)

typedef void (*func_t)(void);

/**
 * @brief This function is used to initialize the scheduler.
 * 
 * @param idle_init A function pointer to the init function of the idle task
 * @param idle_run A function pointer to the run function of the idle task
 */
void scheduler_init(func_t idle_init, func_t idle_run);

/**
 * @brief This function is used to start the scheduler.
 * 
 * @return bool true if the scheduler is started successfully, otherwise false.
 */
bool scheduler_run(void);

/**
 * @brief This function adds a task to the task table of the scheduler.
 * 
 * @param name Is the name of the task.
 * @param init Is a pointer to the initialization function of the task. If there is no init function, set it to NULL.
 * @param run A pointer function to the run function of the task. This function is called intervally.
 * @param priority A number between 0 and 253 which is used when it is supposed to run more than one task. The lower value, the higher priority.
 * @param interval Ts the interval in millisecond which the task is run.
 * @return bool true if the task is added successfully, otherwise false.
 */
bool scheduler_add_task(const char *name, func_t init, func_t run, uint8_t priority, uint16_t interval);

#endif
