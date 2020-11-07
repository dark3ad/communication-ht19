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

    temp = sdcard_init();

    // temp = sdcard_create_file("1");
    // temp = sdcard_append_file("1", "APPENDTEST");
    // temp = sdcard_append_file("1", "ASDGSDGSDG");
    // temp = sdcard_create_file("3");
    // temp = sdcard_create_file("sdfgsdfh");
    // temp = sdcard_create_file("33");
    // temp = sdcard_create_file("31");
    // temp = sdcard_create_file("32");
    // temp = sdcard_create_file("ERRORS");
    // temp = sdcard_append_file("ERRORS", "This is an error!\n");

    uint16_t free_space = sdcard_get_free_space();
    Serial.printf("Free space (Mb) %d\n", free_space);

    filelist_t tmp = sdcard_get_files_list();
    if (tmp.status == OKAY)
    {
        uint8_t i = 0;
        Serial.println(tmp.errors);
        while (strlen(tmp.logs[i]))
        {
            Serial.println(tmp.logs[i]);
            i++;
        }
    }

    uint16_t length = 10;
    char buffer[64] = {};
    temp = sdcard_read_file("1", buffer, length);
    Serial.println(buffer);

    memset(buffer, 0, sizeof(buffer));
    temp = sdcard_read_file("1", buffer, length);
    Serial.println(buffer);

    memset(buffer, 0, sizeof(buffer));
    temp = sdcard_read_file(ERROR_LOG, buffer, length);
    Serial.println(buffer);
    //temp = sdcard_delete_file("01");

    // temp = sdcard_delete_file("3");

    tmp = sdcard_get_files_list();
    if (tmp.status == OKAY)
    {
        uint8_t i = 0;
        Serial.println(tmp.errors);
        while (strlen(tmp.logs[i]))
        {
            Serial.println(tmp.logs[i]);
            i++;
        }
    }

    //sdcard_delete_file("2");
    //sdcard_delete_file(ERROR_LOG);
}

void loop()
{
}