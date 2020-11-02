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
#include <string.h>
#include <stdbool.h>

const int chipSelect = BUILTIN_SDCARD;
File my_file, rootpath;
SdVolume volume;
Sd2Card card;
SdFile root;

uint8_t sdcard_init(void)
{
    if (!SD.begin(chipSelect))
    {
        return SDCARD_BEGIN_ERROR;
    }

    if (!card.init(SPI_HALF_SPEED, chipSelect))
    {
        return SDCARD_BEGIN_ERROR;
    }

    if (!volume.init(card))
    {
        return SDCARD_BEGIN_ERROR;
    }

    rootpath = SD.open("/");

    while (my_file = rootpath.openNextFile())
    {
        _Bool check = false;
        uint8_t temp = atoi(my_file.name());

        for (uint8_t i = 1; i <= DAYS; i++)
        {
            if (i == temp && i != 0)
            {
                check = true;
            }
        }

        uint8_t j = strlen(my_file.name());
        if (j == strlen(ERROR_LOG))
        {
            check = true;
        }

        if (my_file.isDirectory())
        {
            SD.rmdir(my_file.name());

            if (SD.exists(my_file.name()))
            {
                return REMOVE_DIR_ERROR;
            }
        }

        if (!my_file.isDirectory())
        {
            if (!check)
            {
                SD.remove(my_file.name());

                if (SD.exists(my_file.name()))
                {
                    return REMOVE_FILE_ERROR;
                }
            }
        }

        my_file.close();
    }
    rootpath.close();

    return OKAY;
}

uint16_t sdcard_get_free_space(void)
{
    uint32_t size_used = 0;
    rootpath = SD.open("/");

    if (!rootpath)
        return size_used;

    while (my_file = rootpath.openNextFile())
    {
        if (!my_file.isDirectory())
        {
            size_used += my_file.size();
        }
        my_file.close();
    }
    rootpath.close();

    uint32_t volumesize;
    volumesize = volume.blocksPerCluster(); // clusters are collections of blocks
    volumesize *= volume.clusterCount();    // we'll have a lot of clusters
    volumesize /= 2;                        // SD card blocks are always 512 bytes (2 blocks are 1KB)
    volumesize /= 1024;

    size_used *= 0.000001; // Bytes to MB Conversion

    return (uint16_t)(volumesize - size_used);
}

filelist_t sdcard_get_files_list(void)
{
    filelist_t result = {};

    rootpath = SD.open("/");
    if (!rootpath)
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

        rootpath.close();
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
}

uint8_t sdcard_create_file(const char *file_name)
{

    if (!SD.exists(file_name))
    {
        my_file = SD.open(file_name, FILE_WRITE);

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
    my_file = SD.open(file_name, FILE_WRITE);

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

uint8_t sdcard_read_file(char *file_name, char *buffer, uint16_t length)
{

    static uint32_t position = 0xFFFFFFFFU;

    if (!SD.exists(file_name))
    {
        uint8_t i = strlen(file_name);
        if ((i != 2) && (i != strlen(ERROR_LOG)))
        {
            position = 0xFFFFFFFFU;
            return FILE_NOT_EXIST;
        }

        char temp[i + 1] = {0};
        i = 0;
        strcpy(temp, file_name);
        while (file_name[i])
        {
            file_name[i] = toupper(temp[i]);
            i++;
        }
    }

    if (!SD.exists(file_name))
    {
        position = 0xFFFFFFFFU;
        return FILE_NOT_EXIST;
    }

    my_file = SD.open(file_name);
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
