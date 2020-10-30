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
#define FILE_LENGTH (2U) // ex daynumber: 01
#define ERROR_LOG "errors"

typedef struct
{
    uint8_t status;
    char logs[DAYS];
    char errors[sizeof(ERROR_LOG)];
} filelist_t;

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t sdcard_init(void);

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t sdcard_get_free_space(void);

/**
 * @brief 
 * 
 * @return filelist_t 
 */
filelist_t sdcard_get_files_list(void);

/**
 * @brief 
 * 
 * @param file_name 
 * @return uint8_t 
 */
uint8_t sdcard_delete_file(const char *file_name);

/**
 * @brief 
 * 
 * @param file_name 
 * @return uint8_t 
 */
uint8_t sdcard_create_file(const char *file_name);

/**
 * @brief 
 * 
 * @param file_name 
 * @param text 
 * @return uint8_t 
 */
uint8_t sdcard_append_file(const char *file_name, const char *text);

/**
 * @brief 
 * 
 * @param file_name 
 * @param buffer 
 * @param length 
 * @return uint8_t 
 */
uint8_t sdcard_read_file(const char *file_name, char *buffer, uint16_t length);

void sort_strings(char str[DAYS][FILE_LENGTH], int len, int order_type);
void print_2d_array(char str[DAYS][FILE_LENGTH], int len);
void swap_str(char str1[], char str2[]);

#endif /* SDCARD_H */