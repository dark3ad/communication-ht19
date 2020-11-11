/**
 * @file can_service.h
 * @author Bengt Cruz (bengt.cruz@ya.se)
 * @brief A module for the CAN service.
 * @version 0.1
 * @date 2020-11-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CAN_SERVICE_H
#define CAN_SERVICE_H

#define CAN_INTERVAL (10U)

/**
 * @brief This function is used toinitialize the CAN interface and set the filters.
 * 
 */
void can_service_init(void);

/**
 * @brief This function is used to run the CAN service and it shall be run every CAN_INTERVAL.
 * 
 */
void can_service_run(void);

#endif /* CAN_SERVICE_H */