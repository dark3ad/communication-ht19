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
#include <sdcard.h>
#include <string.h>
#include <stdbool.h>

#define ROOT_PATH "/"
#define DIR_VALUE_EXPAND 128

static uint8_t remove_directory(char *init_dir);
static uint8_t erase_memory(void);

static uint32_t volumesize;

SdVolume volume; // DONT WORK WHEN LOCAL IN INIT FUNCTION
Sd2Card card;    // DONT WORK WHEN LOCAL IN INIT FUNCTION
SdFile root;     // DONT WORK WHEN LOCAL IN INIT FUNCTION

uint8_t sdcard_init(void)
{

    if (!SD.begin(BUILTIN_SDCARD))
    {
        return SDCARD_BEGIN_ERROR;
    }

    if (!card.init(SPI_HALF_SPEED, BUILTIN_SDCARD))
    {
        return SDCARD_BEGIN_ERROR;
    }

    if (!volume.init(card))
    {
        return SDCARD_BEGIN_ERROR;
    }

    volumesize = volume.blocksPerCluster();
    volumesize *= volume.clusterCount();
    volumesize /= 2;
    volumesize /= 1024;

    return erase_memory();
}

uint16_t sdcard_get_free_space(void)
{
    uint32_t size_used = 0;
    File root = SD.open("/");
    File entry;

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
    File root = SD.open("/");

    if (!root)
    {
        result.status = OPEN_DIR_ERROR;
    }
    else
    {
        uint8_t j = 0;
        for (uint8_t i = 1; i <= DAYS; i++)
        {
            char name[3] = {};
            sprintf(name, "%02d", i);
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

        result.status = OKAY;
    }
    root.close();

    return result;
}

uint8_t sdcard_delete_file(const char *file_name)
{
    if (!SD.exists(file_name))
    {
        return FILE_NOT_EXIST;
    }
    else
    {
        SD.remove(file_name);
        if (SD.exists(file_name))
        {
            return REMOVE_FILE_ERROR;
        }
    }

    return OKAY;
}

uint8_t sdcard_create_file(const char *file_name)
{
    if (SD.exists(file_name))
    {
        return CREATE_FILE_ERROR;
    }
    else
    {
        File entry = SD.open(file_name, FILE_WRITE);
        entry.close();

        if (!SD.exists(file_name))
        {
            return CREATE_FILE_ERROR;
        }
    }

    return OKAY;
}

uint8_t sdcard_append_file(const char *file_name, const char *text)
{
    File entry = SD.open(file_name, FILE_WRITE);

    if (!entry)
    {
        entry.close();

        return WRITE_FILE_ERROR;
    }
    else
    {
        entry.print(text);
        entry.close();
    }

    return OKAY;
}

uint8_t sdcard_read_file(const char *file_name, char *buffer, uint16_t length)
{

    static uint32_t position = 0xFFFFFFFFU;
    uint8_t i = strlen(file_name);
    char root[i + 1] = {0};

    if (!SD.exists(file_name))
    {
        if ((i != 2) && (i != strlen(ERROR_LOG)))
        {
            position = 0xFFFFFFFFU;
            return FILE_NOT_EXIST;
        }

        i = 0;
        strcpy(root, file_name);
        while (root[i])
        {
            root[i] = toupper(file_name[i]);
            i++;
        }
    }

    if (SD.exists(file_name))
    {
        i = 0;
        strcpy(root, file_name);
        while (root[i])
        {
            root[i] = toupper(file_name[i]);
            i++;
        }
    }

    if (!SD.exists(root))
    {
        position = 0xFFFFFFFFU;
        return FILE_NOT_EXIST;
    }

    File entry = SD.open(root);
    if (!entry)
    {
        position = 0xFFFFFFFFU;
        return OPEN_FILE_ERROR;
    }

    position = (position == 0xFFFFFFFFU) ? 0 : (position + length - 1);

    if (position > entry.size())
    {
        entry.close();
        position = 0xFFFFFFFFU;
    }
    else
    {
        entry.seek(position);
        if (entry.available())
        {
            if (entry.read(buffer, length - 1) < 0)
            {
                entry.close();
                position = 0xFFFFFFFFU;
                return READ_FILE_ERROR;
            }
        }
        else
        {
            position = 0xFFFFFFFFU;
        }
        entry.close();
    }

    return OKAY;
}

static uint8_t erase_memory(void)
{
    File entry;
    File root = SD.open("/");

    while (entry = root.openNextFile())
    {
        char *entry_name = entry.name();

        if (entry.isDirectory())
        {
            char init_dir[DIR_VALUE_EXPAND] = {};
            strcpy(init_dir, ROOT_PATH);
            strcat(init_dir, entry_name);

            remove_directory(init_dir);

            if (SD.exists(entry_name))
            {
                entry.close();
                return REMOVE_DIR_ERROR;
            }
        }
        else
        {
            bool is_valid = false;
            if (!strcmp(ERROR_LOG, entry_name))
            {
                is_valid = true;
            }
            else
            {

                for (uint8_t i = 1; i <= DAYS; i++)
                {
                    char name[FILE_LENGTH] = {};
                    sprintf(name, "%d", i);
                    if (!strcmp(name, entry_name))
                    {
                        is_valid = true;
                        break;
                    }
                }
            }
            if (!is_valid)
            {
                if (!SD.remove(entry_name))
                {
                    entry.close();
                    return REMOVE_FILE_ERROR;
                }
            }
        }
        entry.close();
    }
    root.close();
    return OKAY;
}

static uint8_t remove_directory(char *init_dir)
{
    File root = SD.open(init_dir);

    if (!root)
    {
        return OPEN_DIR_ERROR;
    }
    while (true)
    {
        File entry = root.openNextFile();

        if (entry)
        {
            char *entry_name = entry.name();
            char path[DIR_VALUE_EXPAND] = {};

            if (entry.isDirectory())
            {
                strcpy(path, init_dir);
                strcat(path, ROOT_PATH);
                strcat(path, entry_name);
                strcat(path, ROOT_PATH);
                entry.close();
                remove_directory(path);
            }
            else
            {
                char path_file[DIR_VALUE_EXPAND] = {};
                strcpy(path_file, init_dir);
                strcat(path_file, ROOT_PATH);
                strcat(path_file, entry_name);
                SD.remove(path_file);
            }
            SD.rmdir(path);

            entry.close();
        }
        else
        {
            entry.close();
            break;
        }
    }
    if (!SD.rmdir(init_dir))
    {
        root.close();
        return REMOVE_DIR_ERROR;
    }

    return OKAY;
}