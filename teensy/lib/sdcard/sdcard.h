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

#define OKAY 1
#define SDCARD_BEGIN_ERROR 2
#define CREATE_FILE_ERROR 3
#define OPEN_FILE_ERROR 4
#define FILE_NOT_EXIST 5
#define READ_FILE_ERROR 6
#define WRITE_FILE_ERROR 7
#define REMOVE_FILE_ERROR 8
#define OPEN_DIR_ERROR 9
#define REMOVE_DIR_ERROR 10

#define DAYS (31U)
#define FILE_LENGTH (3U)
#define ERROR_LOG "ERRORS"

typedef struct
{
    uint8_t status;
    char logs[DAYS][FILE_LENGTH];
    char errors[sizeof(ERROR_LOG)];
} filelist_t;

/**
 * @brief this function handles the initialisation of the buildin sdcard
 * 
 * @return uint8_t output of statuscode according to 
 *         macro(2 = SDCARD_BEGIN_ERROR, 1 = OKAY)
 */
uint8_t sdcard_init(void);

/**
 * @brief this function check the amount of (Mb) free space of the sdcard
 * 
 * @return uint16_t output integer of megabytes free space
 */
uint16_t sdcard_get_free_space(void);

/**
 * @brief this function check what files are available on the sdcard and groups them in status, logs and errors
 * 
 * @return filelist_t output struct as uint8_t status, char logs 2D-array and char errors 1D-array and 
 *         statuscode according to macro(9 = OPEN_DIR_ERROR, 1 = OKAY)
 */
filelist_t sdcard_get_files_list(void);

/**
 * @brief this function delete desired file on sdcard
 * 
 * @param file_name input for filename to delete
 * @return uint8_t and statuscode according to 
 *         macro(5 = FILE_NOT_EXIST, 8 = REMOVE_FILE_ERROR, 1 = OKAY)
 */
uint8_t sdcard_delete_file(const char *file_name);

/**
 * @brief this function create desired file on sdcard
 * 
 * @param file_name input for filename to create
 * @return uint8_t and statuscode according to macro(3 = CREATE_FILE_ERROR, 1 = OKAY)
 */
uint8_t sdcard_create_file(const char *file_name);

/**
 * @brief this function write content to desired file on sdcard
 * 
 * @param file_name input for filename to write to
 * @param text input for desired text to write to file
 * @return uint8_t and statuscode according to macro(7 = WRITE_FILE_ERROR, 1 = OKAY)
 */
uint8_t sdcard_append_file(const char *file_name, const char *text);

/**
 * @brief this function read content and length of desired file on sdcard
 * 
 * @param file_name input for desired file to read
 * @param buffer input for desired buffer to read to
 * @param length input for desired length of bytes to read
 * @return uint8_t and statuscode according to 
 *         macro(5 = FILE_NOT_EXIST, 4 = OPEN_FILE_ERROR, 6 = READ_FILE_ERROR, 1 = OKAY)
 */
uint8_t sdcard_read_file(char *file_name, char *buffer, uint16_t length);

#endif /* SDCARD_H */