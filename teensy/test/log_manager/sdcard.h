/**
 * @file sdcard.h
 * @author Patrik Gustafsson (patrik-emil.gustafsson@yaelev.se)
 * @brief This library handles SD card functionality
 * @version 0.1
 * @date 2020-10-26
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef SDCARD_H
#define SDCARD_H

#include <stdint.h>

#define SUCCESS 1
#define FILE_EXIST 2
#define FILE_NOT_EXIST 3
#define OPEN_DIR_ERROR 4
#define OPEN_FILE_ERROR 5
#define READ_FILE_ERROR 6
#define WRITE_FILE_ERROR 7
#define REMOVE_DIR_ERROR 8
#define INVALID_FILE_NAME 9
#define CREATE_FILE_ERROR 10
#define REMOVE_FILE_ERROR 11
#define SDCARD_BEGIN_ERROR 12

#define DAYS (31U)
#define FILE_LENGTH (3U)
#define ERROR_LOG "ERRORS"

/**
 * @brief This function handles the initialisation of the builtin SD card
 * 
 * @return uint8_t The status code: SUCCESS | SDCARD_BEGIN_ERROR | OPEN_DIR_ERROR | REMOVE_FILE_ERROR | OPEN_DIR_ERROR | REMOVE_DIR_ERROR
 */
uint8_t sdcard_init(void);

/**
 * @brief This function check the amount of (Mb) free space of the SD card
 * 
 * @return uint16_t The free space in megabytes
 */
uint16_t sdcard_get_free_space(void);

/**
 * @brief This function deletes desired file on the SD card
 * 
 * @param file_name input for filename to delete
 * @return uint8_t The status code: FILE_NOT_EXIST, REMOVE_FILE_ERROR, SUCCESS
 */
uint8_t sdcard_delete_file(const char *file_name);

/**
 * @brief This function creates desired file on the SD card
 * 
 * @param file_name The filename to create
 * @return uint8_t The status code: INVALID_FILE_NAME | FILE_EXIST | CREATE_FILE_ERROR | SUCCESS
 */
uint8_t sdcard_create_file(const char *file_name);

/**
 * @brief This function writes text to desired file on the SD card
 * 
 * @param file_name The filename to write to
 * @param text The text to write to the file
 * @return uint8_t The status code: INVALID_FILE_NAME | FILE_NOT_EXIST | OPEN_FILE_ERROR | WRITE_FILE_ERROR | SUCCESS
 */
uint8_t sdcard_append_file(const char *file_name, const char *text);

#endif /* SDCARD_H */