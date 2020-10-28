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

    Serial.printf("Initializing SD card...\n");

    if (!SD.begin(chipSelect))
    {
        PRINTF("%s\n", "Initialization failed! Things to check:");
        Serial.println("* is a card inserted?");
        return SDCARD_BEGIN_ERROR;
    }

    if (!card.init(SPI_HALF_SPEED, chipSelect))
    {
        Serial.println("Initialization failed! Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");
        return SDCARD_BEGIN_ERROR;
    }

    switch (card.type()) // REMOVE FROM PRODUCTION
    {

    case SD_CARD_TYPE_SD1:

        Serial.println("SD1");

        break;

    case SD_CARD_TYPE_SD2:

        Serial.println("SD2");

        break;

    case SD_CARD_TYPE_SDHC:

        Serial.println("SDHC");

        break;

    default:

        Serial.println("Unknown");
    }

    if (!volume.init(card))
    {
        Serial.println("Error. Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card.");
        return SDCARD_BEGIN_ERROR;
    }

    Serial.print("Clusters:          ");
    Serial.println(volume.clusterCount());
    Serial.print("Blocks x Cluster:  ");
    Serial.println(volume.blocksPerCluster());
    Serial.print("Total Blocks:      ");
    Serial.println(volume.blocksPerCluster() * volume.clusterCount());

    // print the type and size of the first FAT-type volume

    uint32_t volumesize;

    Serial.print("Volume type is:    FAT");
    Serial.println(volume.fatType(), DEC);

    volumesize = volume.blocksPerCluster(); // clusters are collections of blocks
    volumesize *= volume.clusterCount();    // we'll have a lot of clusters
    volumesize /= 2;                        // SD card blocks are always 512 bytes (2 blocks are 1KB)

    Serial.print("Volume size (Kb):  ");
    Serial.println(volumesize);

    Serial.print("Volume size (Mb):  ");
    volumesize /= 1024;
    Serial.println(volumesize);

    Serial.print("Volume size (Gb):  ");
    Serial.println((float)volumesize / 1024.0);

    Serial.println("Initialization done.");

    return OKAY;
}

uint16_t sdcard_get_free_space(void) ///////////////// HELP /////////////////
{
    //REQ: It shall be possible to get the free space in MB of the SD card.

    uint16_t free_space;

    // GET TOTAL MB SIZE FROM INIT.
    // READ ALL THE FILES AND SUBTRACT FROM RESULT ABOVE

    return free_space;
}
filelist_t sdcard_get_files_list(void) ///////////////// HELP /////////////////
{
    //REQ: It shall be possible to get the SORTED list of files on the SD card

    //sortArray
    //SPRINTF __format_arg
    filelist_t tmp;

    Serial.println("Listing files #1..."); // Solution #1

    root.openRoot(volume);
    root.ls(LS_R | LS_DATE | LS_SIZE);
    root.close();

    Serial.println("Listing files #2..."); // Solution #2

    myFile = SD.open("/");

    while (true)
    {
        File entry = myFile.openNextFile();

        if (!entry)
        {
            break;
        }

        Serial.println(entry.name());

        entry.close();
    }

    Serial.println("File listing... done.");

    return tmp;
}

uint8_t sdcard_delete_file(const char *file_name)
{
    //REQ: It shall be possible to delet a file.

    if (SD.exists(file_name))
    {
        Serial.printf("Removing %s...\n", file_name);
        SD.remove(file_name);
    }
    else
    {
        Serial.printf("%s doesn't exist.\n", file_name);
        return FILE_NOT_EXIST;
    }

    if (SD.exists(file_name))
    {
        Serial.printf("File %s NOT removed. Error.\n", file_name);
        return REMOVE_FILE_ERROR;
    }
    else
    {
        Serial.printf("File %s removed.\n", file_name);
        return OKAY;
    }
}

uint8_t sdcard_create_file(const char *file_name)
{
    //REQ: It shall be possible create a file if it does not exist.

    if (!SD.exists(file_name))
    {

        Serial.printf("Creating %s...\n", file_name);

        myFile = SD.open(file_name, FILE_WRITE);

        myFile.close();

        if (SD.exists(file_name))
        {
            Serial.printf("File %s created.\n", file_name);
            return OKAY;
        }
        else
        {
            Serial.printf("File %s wasnt't created. Error.\n", file_name);
            return CREATE_FILE_ERROR;
        }
    }
    else
    {
        Serial.printf("File %s already exists.\n", file_name);
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
        Serial.printf("Writing to %s...\n", file_name);
        myFile.println(text);

        myFile.close();
        Serial.println("Writing done.");
        return OKAY;
    }
    else
    {
        Serial.printf("Error writing to file %s.\n", file_name);
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
        Serial.printf("Reading file %s...\n", file_name);

        memset(buffer, 0, length);

        if (myFile.available())
        {
            myFile.read(buffer, length);
        }

        myFile.close();

        Serial.printf("Reading done.\n", file_name);

        return OKAY;
    }
    else
    {
        Serial.printf("Reading file %s error.\n", file_name);
        return READ_FILE_ERROR;
    }

    return READ_FILE_ERROR;
}
