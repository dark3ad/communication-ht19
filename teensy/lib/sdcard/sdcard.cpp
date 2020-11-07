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

static uint8_t remove_directory(File entry, String init_path);
static uint8_t erase_memory(void);

static uint32_t volumesize;

SdVolume volume; // FLYTTA TILL INIT-FUNKTIONEN & DEBUGGA, NÄR ALLT ANNAT ÄR ÅTGÄRDAT
Sd2Card card;    // FLYTTA TILL INIT-FUNKTIONEN & DEBUGGA, NÄR ALLT ANNAT ÄR ÅTGÄRDAT
SdFile root;     // FLYTTA TILL INIT-FUNKTIONEN & DEBUGGA, NÄR ALLT ANNAT ÄR ÅTGÄRDAT

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

    // Open the entry
    // If it is a file => check if not valid => remove it
    // else if it is a folder and it is empty => remove it otherwise go to the first step

    File entry;
    File root = SD.open("/");
    String root_path = "/";

    while (entry = root.openNextFile())
    {
        char *entry_name = entry.name();

        if (entry.isDirectory())
        {
            String init_path = root_path + entry.name() + root_path;
            remove_directory(entry, init_path);

            if (!SD.rmdir(entry_name))
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
                    sprintf(name, "%d", i); // sprintf(name, "%02d", i); <-- DELETES everything under 10 due to %02d, correction to %d
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

static uint8_t remove_directory(File dir, String init_path)
{

    // if the directory is empty => remove it
    // else open the entry and get the next entry and get the name of the entry
    // if the entry is a file remove it
    // else if the entry is a folder - call itself by the entry name

    uint8_t status = OKAY;
    String root_path = "/";

    while (true)
    {
        File entry = dir.openNextFile();
        String localpath;

        if (entry)
        {
            if (entry.isDirectory())
            {
                localpath = init_path + entry.name() + root_path + '\0';
                char folder_buf[localpath.length()];
                localpath.toCharArray(folder_buf, localpath.length());
                status = remove_directory(entry, folder_buf);

                if (!SD.rmdir(folder_buf))
                {
                    status = REMOVE_DIR_ERROR;
                }
            }
            else
            {
                localpath = init_path + entry.name() + '\0';
                char char_buf[localpath.length()];
                localpath.toCharArray(char_buf, localpath.length());

                if (!SD.remove(char_buf))
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