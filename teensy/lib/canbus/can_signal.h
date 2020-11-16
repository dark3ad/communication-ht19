/**
 * @file can_signal.h
 * @author Bengt Cruz (bengt.cruz@ya.se)
 * @brief A module to write(pack) and read(unpack) a signal with a start position, length and value to/from a specific message
 * @version 0.1
 * @date 2020-11-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CAN_SIGNAL_H
#define CAN_SIGNAL_H

#include <stdint.h>

/**
 * @brief Similiar to packing a message this function will write a signal from the message index is given. 
 * 
 * @param msg_index The message index
 * @param start The start position of the signal in the message
 * @param length The length of the signal
 * @param value The value of the signal
 */
void can_signal_write(uint8_t msg_index, uint8_t start, uint8_t length, uint64_t value);

/**
 * @brief Similar to unpacking a message this function will read a signal from the message index is given. 
 * 
 * @param msg_index The message index
 * @param start The start position of the signal in the message
 * @param length The length of the signal
 * @return uint64_t The return value of the signal from the CAN bus
 */
uint64_t can_signal_read(uint8_t msg_index, uint8_t start, uint8_t length);

#endif /* CAN_SIGNAL_H */