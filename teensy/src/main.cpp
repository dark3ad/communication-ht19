#include <bsp.h>
#include <sdcard.h>

void setup()
{

    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect.
    }

    uint8_t temp;

    uint16_t length;
    char *buffer;

    temp = sdcard_init();
    //uint16_t free_space = sdcard_get_free_space();
    temp = sdcard_create_file("01");
    temp = sdcard_append_file("01", "APPENDTEST");
    temp = sdcard_create_file("02");
    filelist_t tmp = sdcard_get_files_list();

    temp = sdcard_read_file("01", buffer, length);
    temp = sdcard_delete_file("01");
}

void loop()
{
}