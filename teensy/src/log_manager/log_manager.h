/**
 * @file log_manager.h
 * @author Patrik Gustafsson (patrik-emil.gustafsson@yaelev.se)
 * @brief Log manager handles logging changes on canbus. Includes functionality for Date-dependent Log rotation, Log analysis, Error handling
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#define LOG_MANAGER_INTERVAL (1000U)

/**
 * @brief this function initialises the logmanager and its necessesary components
 * 
 */
void log_manager_init(void);

/**
 * @brief this function is the runnable component of logmanager
 * 
 */
void log_manager_run(void);

#endif /* LOG_MANAGER_H */