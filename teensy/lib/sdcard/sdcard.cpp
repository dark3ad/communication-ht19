/**
 * @file sdcard.cpp
 * @author Patrik Gustafsson (patrik-emil.gustafsson@yaelev.se)
 * @brief This library handles SD card functionality
 * @version 0.1
 * @date 2020-10-26
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <SD.h>
#include <bsp.h>
#include <sdcard.h>
#include <stdbool.h>
#include <bsp_time.h>

#define ROOT_PATH "/"
#define DIR_VALUE_EXPAND (256U)

static uint8_t erase_memory(void);
static bool check_file_name(const char *name);
static uint8_t remove_directory(char *init_dir);
static void date_time(uint16_t *date, uint16_t *time);

static uint32_t volumesize;

uint8_t sdcard_init(void)
{
    SdFile root;
    Sd2Card card;
    SdVolume volume;
    uint8_t status = SDCARD_BEGIN_ERROR;

    if (card.init(SPI_HALF_SPEED, BUILTIN_SDCARD) && volume.init(card) && root.openRoot(volume))
    {
        volumesize = volume.blocksPerCluster();
        volumesize *= volume.clusterCount();
        volumesize /= 2048;

        if (SD.begin(BUILTIN_SDCARD))
        {
            status = erase_memory();
        }
    }

    return status;
}

uint16_t sdcard_get_free_space(void)
{
    File entry;
    uint32_t size_used = 0;
    File root = SD.open(ROOT_PATH);

    if (!root)
    {
        return size_used;
    }

    while (entry = root.openNextFile())
    {
        if (!entry.isDirectory())
        {
            size_used += entry.size();
        }
        entry.close();
    }
    root.close();

    size_used /= (1024 * 1024);

    return (uint16_t)(volumesize - size_used);
}

filelist_t sdcard_get_files_list(void)
{
    filelist_t result = {};
    File root = SD.open(ROOT_PATH);

    if (!root)
    {
        result.status = OPEN_DIR_ERROR;
    }
    else
    {
        uint8_t j = 0;
        for (uint8_t i = 1; i <= DAYS; i++)
        {
            char name[FILE_LENGTH] = {};
            sprintf(name, "%02d", i); // 02d?
            if (SD.exists(name))
            {
                strcpy(result.logs[j], name);
                j++;
            }
        }

        if (SD.exists(ERROR_LOG))
        {
            strcpy(result.errors, ERROR_LOG);
        }

        result.status = SUCCESS;
    }
    root.close();

    return result;
}

uint8_t sdcard_create_file(const char *file_name)
{
    uint8_t status = INVALID_FILE_NAME;

    if (check_file_name(file_name))
    {
        if (SD.exists(file_name))
        {
            status = FILE_EXIST;
        }
        else
        {
            SdFile::dateTimeCallback(date_time);
            File file = SD.open(file_name, O_CREAT | O_WRITE);
            if (file)
            {
                file.close();
                status = SUCCESS;
            }
            else
            {
                status = CREATE_FILE_ERROR;
            }
        }
    }

    return status;
}

uint8_t sdcard_delete_file(const char *file_name)
{
    uint8_t status = FILE_NOT_EXIST;

    if (SD.exists(file_name))
    {
        status = SD.remove(file_name) ? SUCCESS : REMOVE_FILE_ERROR;
    }

    return status;
}

uint8_t sdcard_append_file(const char *file_name, const char *text)
{
    uint8_t status = SUCCESS;
    if (!check_file_name(file_name))
    {
        status = INVALID_FILE_NAME;
    }
    else if (!SD.exists(file_name))
    {
        status = FILE_NOT_EXIST;
    }
    else
    {
        File file = SD.open(file_name, FILE_WRITE);
        if (!file)
        {
            status = OPEN_FILE_ERROR;
        }
        else
        {
            if (strlen(text) != file.print(text))
            {
                status = WRITE_FILE_ERROR;
            }
            file.close();
        }
    }

    return status;
}

uint8_t sdcard_read_file(const char *name, char *buffer, uint16_t length)
{
    uint8_t status = SUCCESS;
    static uint32_t position = 0xFFFFFFFFU;
    if (!check_file_name(name))
    {
        position = 0xFFFFFFFFU;
        status = INVALID_FILE_NAME;
    }
    else if (!SD.exists(name))
    {
        position = 0xFFFFFFFFU;
        status = FILE_NOT_EXIST;
    }
    else
    {
        File file = SD.open(name);
        if (!file)
        {
            position = 0xFFFFFFFFU;
            status = OPEN_FILE_ERROR;
        }
        else
        {
            memset(buffer, 0, length);
            position = (position == 0xFFFFFFFFU) ? 0 : (position + length - 1);

            if (position > file.size())
            {
                position = 0xFFFFFFFFU;
            }
            else
            {
                file.seek(position);
                if (file.available())
                {
                    if (file.read(buffer, length - 1) < 0)
                    {
                        position = 0xFFFFFFFFU;
                        status = READ_FILE_ERROR;
                    }
                }
                else
                {
                    position = 0xFFFFFFFFU;
                    status = READ_FILE_ERROR;
                }
            }
            file.close();
        }
    }

    return status;
}

/**
 * @brief This function is used to remove all the invalid files and directories.
 * @remark The function can not remove read-only files and directories.
 * 
 * @return uint8_t The status code: SUCCESS | OPEN_DIR_ERROR | REMOVE_FILE_ERROR | OPEN_DIR_ERROR | REMOVE_DIR_ERROR
 */
static uint8_t erase_memory(void)
{
    uint8_t status = SUCCESS;
    File root = SD.open(ROOT_PATH);

    if (!root)
    {
        status = OPEN_DIR_ERROR;
    }
    else
    {
        File entry;
        while (entry = root.openNextFile())
        {
            char *name = entry.name();
            if (entry.isDirectory())
            {
                status = remove_directory(name);
            }
            else if (!check_file_name(name))
            {
                if (!SD.remove(name))
                {
                    status = REMOVE_FILE_ERROR;
                }
            }
            entry.close();

            if (status != SUCCESS)
            {
                break;
            }
        }
        root.close();
    }

    return status;
}

static uint8_t remove_directory(char *init_dir)
{
    if (!SD.rmdir(init_dir))
    {
        File root = SD.open(init_dir);

        if (!root)
        {
            return OPEN_DIR_ERROR;
        }

        File entry;

        while (entry = root.openNextFile())
        {
            uint8_t status = SUCCESS;
            char entry_name[DIR_VALUE_EXPAND] = {};

            strcpy(entry_name, init_dir);
            strcat(entry_name, ROOT_PATH);
            strcat(entry_name, entry.name());

            if (entry.isDirectory())
            {
                status = remove_directory(entry_name);
            }
            else if (!SD.remove(entry_name))
            {
                PRINTF("[F] Failed to remove %s\n", entry_name);
                status = REMOVE_FILE_ERROR;
            }
            entry.close();

            if (SUCCESS != status)
            {
                return status;
            }

            entry.close();
        }
        root.close();

        if (!SD.rmdir(init_dir))
        {
            PRINTF("[D] Failed to remove %s\n", init_dir);
            return REMOVE_DIR_ERROR;
        }
    }

    return SUCCESS;
}

static bool check_file_name(const char *name)
{
    bool valid = false;
    if (!strcmp(name, ERROR_LOG))
    {
        valid = true;
    }
    else
    {
        for (uint8_t i = 1; i <= DAYS; i++)
        {
            char temp[FILE_LENGTH] = {};
            sprintf(temp, "%02d", i);
            if (!strcmp(temp, name))
            {
                valid = true;
                break;
            }
        }
    }
    return valid;
}

static void date_time(uint16_t *date, uint16_t *time)
{
    *date = FAT_DATE(bsp_year(), bsp_month(), bsp_day());
    *time = FAT_TIME(bsp_hour(), bsp_minute(), bsp_second());
}
