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

static uint8_t remove_directory(File dir, String temp_path);
static uint8_t erase_memory(void);

static String rootpath = "/";
static uint32_t volumesize;

SdVolume volume;
Sd2Card card;
SdFile root;

uint8_t sdcard_init(void)
{
    uint8_t status = SDCARD_BEGIN_ERROR;

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

    status = erase_memory();

    return status;
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
        File entry = SD.open(file_name, FILE_WRITE);
        entry.close();

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
    File entry = SD.open(file_name, FILE_WRITE);

    if (entry)
    {
        entry.print(text);
        entry.close();

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

    File entry = SD.open(temp);
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
    uint8_t status = REMOVE_FILE_ERROR;
    File root = SD.open("/");
    File entry;

    while (entry = root.openNextFile())
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

        if (entry.isDirectory())
        {
            SD.rmdir(entry.name());

            if (SD.exists(entry.name()))
            {
                String temp_path = rootpath + entry.name() + rootpath;
                status = remove_directory(entry, temp_path);
            }

            SD.rmdir(entry.name());
        }

        if (!entry.isDirectory())
        {
            if (!check)
            {
                SD.remove(entry.name());

                if (SD.exists(entry.name()))
                {
                    status = REMOVE_FILE_ERROR;
                }
                else
                {
                    status = OKAY;
                }
            }
        }

        entry.close();
    }
    root.close();

    return status;
}

static uint8_t remove_directory(File dir, String temp_path)
{
    uint8_t status = REMOVE_DIR_ERROR;

    while (true)
    {
        File entry = dir.openNextFile();
        String localpath;

        if (entry)
        {
            if (entry.isDirectory())
            {
                localpath = temp_path + entry.name() + rootpath + '\0';
                char folder_buf[localpath.length()];
                localpath.toCharArray(folder_buf, localpath.length());
                status = remove_directory(entry, folder_buf);

                if (SD.rmdir(folder_buf))
                {
                    status = OKAY;
                }
                else
                {
                    status = REMOVE_DIR_ERROR;
                }
            }
            else
            {
                Serial.println(localpath);
                localpath = temp_path + entry.name() + '\0';
                char char_buf[localpath.length()];
                localpath.toCharArray(char_buf, localpath.length());

                if (SD.remove(char_buf))
                {
                    status = OKAY;
                }
                else
                {
                    status = REMOVE_FILE_ERROR;
                }
            }
        }
        else
        {
            entry.close();
            break;
        }
    }
    return status;
}