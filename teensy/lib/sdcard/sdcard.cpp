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
#include <stdio.h>
#include <sdcard.h>

const int chipSelect = BUILTIN_SDCARD;
File myFile;
SdVolume volume;
Sd2Card card;
SdFile root;

uint8_t sdcard_init(void)
{
    //REQ: It shall be possible to initialize the SD card and delete all invalid files and folders

    // CHECK 01-31 + ERROR (LOG)
    // DELETE EVERYTHING ELSE.

    PRINTF("%s\n", "Initializing SD card...\n");

    if (!SD.begin(chipSelect))
    {
        PRINTF("%s\n", "Initialization failed! Things to check:");
        PRINTF("%s\n", "* is a card inserted?");
        return SDCARD_BEGIN_ERROR;
    }

    if (!card.init(SPI_HALF_SPEED, chipSelect))
    {
        PRINTF("%s\n", "Initialization failed! Things to check:");
        PRINTF("%s\n", "* is a card inserted?");
        PRINTF("%s\n", "* is your wiring correct?");
        PRINTF("%s\n", "* did you change the chipSelect pin to match your shield or module?");
        return SDCARD_BEGIN_ERROR;
    }

    switch (card.type()) // REMOVE FROM PRODUCTION
    {

    case SD_CARD_TYPE_SD1:

        PRINTF("%s\n", "SD1");

        break;

    case SD_CARD_TYPE_SD2:

        PRINTF("%s\n", "SD2");

        break;

    case SD_CARD_TYPE_SDHC:

        PRINTF("%s\n", "SDHC");

        break;

    default:

        PRINTF("%s\n", "Unknown");
    }

    if (!volume.init(card))
    {
        PRINTF("%s\n", "Error. Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card.");
        return SDCARD_BEGIN_ERROR;
    }

    PRINTF("%s\n", "Clusters:          ");
    PRINTF("%d\n", volume.clusterCount());
    PRINTF("%s\n", "Blocks x Cluster:  ");
    PRINTF("%d\n", volume.blocksPerCluster());
    PRINTF("%s\n", "Total Blocks:      ");
    PRINTF("%d\n", volume.blocksPerCluster() * volume.clusterCount());

    // print the type and size of the first FAT-type volume

    uint32_t volumesize;

    PRINTF("%s\n", "Volume type is:    FAT");
    PRINTF("%d\n", volume.fatType(), DEC);

    volumesize = volume.blocksPerCluster(); // clusters are collections of blocks
    volumesize *= volume.clusterCount();    // we'll have a lot of clusters
    volumesize /= 2;                        // SD card blocks are always 512 bytes (2 blocks are 1KB)

    PRINTF("%s\n", "Volume size (Kb):  ");
    PRINTF("%d\n", volumesize);

    PRINTF("%s\n", "Volume size (Mb):  ");
    volumesize /= 1024;
    PRINTF("%d\n", volumesize);

    PRINTF("%s\n", "Volume size (Gb):  ");
    PRINTF("%f\n", (float)volumesize / 1024.0);

    PRINTF("%s\n", "Initialization done.");

    return OKAY;
}

uint16_t sdcard_get_free_space(void)
{
    //REQ: It shall be possible to get the free space in MB of the SD card.

    uint32_t size_used = 0;
    File root = SD.open("/");

    if (!root)
        return size_used;
    while (myFile = root.openNextFile())
    {
        if (!myFile.isDirectory())
        {
            size_used += myFile.size();
        }
        myFile.close();
    }
    root.close();

    uint32_t volumesize;

    volumesize = volume.blocksPerCluster(); // clusters are collections of blocks
    volumesize *= volume.clusterCount();    // we'll have a lot of clusters
    volumesize /= 2;                        // SD card blocks are always 512 bytes (2 blocks are 1KB)

    PRINTF("%s\n", "Free space (Mb):  ");
    volumesize /= 1024;

    size_used *= 0.000001; // Bytes to MB Conversion

    PRINTF("%d\n", (uint16_t)(volumesize - size_used));

    return (uint16_t)(volumesize - size_used);
}

filelist_t sdcard_get_files_list(void) ///////////////// HELP /////////////////
{
    //REQ: It shall be possible to get the SORTED list of files on the SD card

    //sortArray
    //SPRINTF __format_arg
    filelist_t tmp;

    PRINTF("%s\n", "Listing files #1..."); // Solution #1

    root.openRoot(volume);
    root.ls(LS_R | LS_DATE | LS_SIZE);
    root.close();

    PRINTF("%s\n", "Listing files #2..."); // Solution #2

    myFile = SD.open("/");

    while (true)
    {
        File entry = myFile.openNextFile();

        if (!entry)
        {
            break;
        }

        PRINTF("%s\n", entry.name());

        entry.close();
    }

    PRINTF("%s\n", "File listing... done.");

    return tmp;
}

uint8_t sdcard_delete_file(const char *file_name)
{
    //REQ: It shall be possible to delet a file.

    if (SD.exists(file_name))
    {
        PRINTF("%s\n", "Removing %s...\n", file_name);
        SD.remove(file_name);
    }
    else
    {
        PRINTF("%s\n", "%s doesn't exist.\n", file_name);
        return FILE_NOT_EXIST;
    }

    if (SD.exists(file_name))
    {
        PRINTF("%s\n", "File %s NOT removed. Error.\n", file_name);
        return REMOVE_FILE_ERROR;
    }
    else
    {
        PRINTF("%s\n", "File %s removed.\n", file_name);
        return OKAY;
    }
}

uint8_t sdcard_create_file(const char *file_name)
{
    //REQ: It shall be possible create a file if it does not exist.

    if (!SD.exists(file_name))
    {

        PRINTF("%s\n", "Creating %s...\n", file_name);

        myFile = SD.open(file_name, FILE_WRITE);

        myFile.close();

        if (SD.exists(file_name))
        {
            PRINTF("%s\n", "File %s created.\n", file_name);
            return OKAY;
        }
        else
        {
            PRINTF("%s\n", "File %s wasnt't created. Error.\n", file_name);
            return CREATE_FILE_ERROR;
        }
    }
    else
    {
        PRINTF("%s\n", "File %s already exists.\n", file_name);
        return CREATE_FILE_ERROR;
    }

    return CREATE_FILE_ERROR;
}

uint8_t sdcard_append_file(const char *file_name, const char *text)
{
    //REQ: It shall be possible to write to a file.

    myFile = SD.open(file_name, FILE_WRITE);

    if (myFile)
    {
        PRINTF("%s\n", "Writing to %s...\n", file_name);
        myFile.println(text);

        myFile.close();
        PRINTF("%s\n", "Writing done.");
        return OKAY;
    }
    else
    {
        PRINTF("%s\n", "Error writing to file %s.\n", file_name);
        return WRITE_FILE_ERROR;
    }

    return WRITE_FILE_ERROR;
}

uint8_t sdcard_read_file(const char *file_name, char *buffer, uint16_t length) ///////////////// HELP  HOW BUFFER + LENGTH /////////////////
{
    //REQ: It shall be possible to read a file.

    myFile = SD.open(file_name, FILE_READ);

    if (myFile)
    {
        PRINTF("%s\n", "Reading file %s...\n", file_name);

        memset(buffer, 0, length);

        if (myFile.available())
        {
            myFile.read(buffer, length);
        }

        myFile.close();

        PRINTF("%s\n", "Reading done.\n", file_name);

        return OKAY;
    }
    else
    {
        PRINTF("%s\n", "Reading file %s error.\n", file_name);
        return READ_FILE_ERROR;
    }

    return READ_FILE_ERROR;
}
