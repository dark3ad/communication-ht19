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

static uint8_t remove_directory(char *name);
static uint8_t erase_memory(void);

static uint32_t volumesize;
static SdVolume volume;
static Sd2Card card;
static SdFile root;

uint8_t sdcard_init(void)
{
    uint8_t status = OKAY;

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
    volumesize = volume.blocksPerCluster(); // clusters are collections of blocks
    volumesize *= volume.clusterCount();    // we'll have a lot of clusters
    volumesize /= 2;                        // SD card blocks are always 512 bytes (2 blocks are 1KB)
    volumesize /= 1024;

    status = erase_memory();

    return status;
}

uint16_t sdcard_get_free_space(void)
{
    uint32_t size_used = 0;
    File root = SD.open("/");
    File my_file;

    if (!root)
    {
        return size_used;
    }

    while (my_file = root.openNextFile())
    {
        if (!my_file.isDirectory())
        {
            size_used += my_file.size();
        }
        my_file.close();
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

        root.close();
    }

    return result;
}

uint8_t sdcard_delete_file(const char *file_name)
{

    if (SD.exists(file_name))
    {
        SD.remove(file_name);
    }
    else
    {
        return FILE_NOT_EXIST;
    }

    if (SD.exists(file_name))
    {
        return REMOVE_FILE_ERROR;
    }
    else
    {
        return OKAY;
    }

    return REMOVE_FILE_ERROR;
}

uint8_t sdcard_create_file(const char *file_name)
{

    if (!SD.exists(file_name))
    {
        File my_file = SD.open(file_name, FILE_WRITE);
        my_file.close();

        if (SD.exists(file_name))
        {
            return OKAY;
        }
        else
        {
            return CREATE_FILE_ERROR;
        }
    }
    else
    {
        return CREATE_FILE_ERROR;
    }

    return CREATE_FILE_ERROR;
}

uint8_t sdcard_append_file(const char *file_name, const char *text)
{
    File my_file = SD.open(file_name, FILE_WRITE);

    if (my_file)
    {
        my_file.print(text);
        my_file.close();

        return OKAY;
    }
    else
    {
        return WRITE_FILE_ERROR;
    }

    return WRITE_FILE_ERROR;
}

uint8_t sdcard_read_file(const char *file_name, char *buffer, uint16_t length)
{

    static uint32_t position = 0xFFFFFFFFU;
    uint8_t i = strlen(file_name);
    char temp[i + 1] = {0};

    if (!SD.exists(file_name))
    {
        if ((i != 2) && (i != strlen(ERROR_LOG)))
        {
            position = 0xFFFFFFFFU;
            return FILE_NOT_EXIST;
        }

        i = 0;
        strcpy(temp, file_name);
        while (temp[i])
        {
            temp[i] = toupper(file_name[i]);
            i++;
        }
    }

    if (SD.exists(file_name))
    {
        i = 0;
        strcpy(temp, file_name);
        while (temp[i])
        {
            temp[i] = toupper(file_name[i]);
            i++;
        }
    }

    if (!SD.exists(temp))
    {
        position = 0xFFFFFFFFU;
        return FILE_NOT_EXIST;
    }

    File my_file = SD.open(temp);
    if (!my_file)
    {
        position = 0xFFFFFFFFU;
        return OPEN_FILE_ERROR;
    }

    position = (position == 0xFFFFFFFFU) ? 0 : (position + length - 1);

    if (position > my_file.size())
    {
        my_file.close();
        position = 0xFFFFFFFFU;
    }
    else
    {
        my_file.seek(position);
        if (my_file.available())
        {
            if (my_file.read(buffer, length - 1) < 0)
            {
                my_file.close();
                position = 0xFFFFFFFFU;
                return READ_FILE_ERROR;
            }
        }
        else
        {
            position = 0xFFFFFFFFU;
        }
        my_file.close();
    }

    return OKAY;
}

static uint8_t erase_memory(void)
{
    File entry;
    uint8_t status = OKAY;
    File root = SD.open("/");

    if (!root)
    {
        return OPEN_DIR_ERROR;
    }
    while (entry = root.openNextFile())
    {
        if (entry.isDirectory())
        {
            status = remove_directory(entry.name());
        }
        if (!entry.isDirectory())
        {
            _Bool check = false;
            uint8_t temp = atoi(entry.name());

            for (uint8_t i = 1; i <= DAYS; i++)
            {
                if (i == temp && i != 0)
                {
                    check = true;
                }
            }

            uint8_t j = strlen(entry.name());
            if (j == strlen(ERROR_LOG))
            {
                check = true;
            }
            if (!check)
            {

                status = SD.remove(entry.name()) ? OKAY : REMOVE_FILE_ERROR;

                if (SD.exists(entry.name()))
                {
                    return REMOVE_FILE_ERROR;
                }
            }
        }
        entry.close();

        if (status != OKAY)
        {
            break;
        }
    }
    root.close();

    return status;
}

static uint8_t remove_directory(char *name)
{
    if (!SD.rmdir(name))
    {
        File entry;
        File root = SD.open(name);
        if (!root)
        {
            return OPEN_DIR_ERROR;
        }
        while (entry = root.openNextFile())
        {
            char entryName[32];
            sprintf(entryName, "%s/%s", name, entry.name());
            if (entry.isDirectory())
            {
                remove_directory(entryName);
            }
            else
            {
                SD.remove(entryName);
            }
            entry.close();
        }
        root.close();

        if (!SD.rmdir(name))
        {
            return REMOVE_DIR_ERROR;
        }
    }
    return OKAY;
}