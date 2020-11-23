#ifdef TARGET
#include<Arduino.h>
#endif

#include<unity.h>
#include<bsp_io.h>
#include<sdcard.h>
#include<terminal.h>
#include<stdio.h>
#include<string.h>

void set_temperature_calibration_max(uint8_t value)
{
    printf("\nTemperature Calibration Max updated: %d.\n", value);
}

void set_temperature_calibration_min(uint8_t value)
{
    printf("\nTemperature Calibration Min updated: %d.\n", value);
}

void set_temperature_target_min(uint8_t value)
{
    printf("\nTemperature Target Min updated: %d.\n", value);
}

void set_temperature_target_max(uint8_t value)
{
    printf("\nTemperature Target Max updated: %d.\n", value);
}

uint8_t sdcard_read_file(const char *file_name, char *buffer, uint16_t length)
{
    memmove(buffer, "Hello World!", 13);
    return SUCCESS;
}

uint8_t get_sdcard_status()
{
  return SUCCESS;
}

filelist_t sdcard_get_files_list(void) 
{
  filelist_t new_filelist = {
    SUCCESS,
    {"F1", "F2"}
  };
  return new_filelist;
}

uint8_t bsp_serial_available(void)
{
  return 1;
}

void bsp_serial_read(char * buffer)
{
  scanf("%s", buffer);
}

void setUp()
{

}

void tearDown()
{

}

#ifdef TARGET
void run()
#else
int run()
#endif
{
  UNITY_BEGIN();

  terminal_initialize();
  terminal_run();

#ifdef TARGET
  UNITY_END();
#else
  return UNITY_END();
#endif
}

#ifdef TARGET
void loop()
{
}

void setup()
{
  delay(3000);
  run();
}
#else
int main()
{
  run();
}
#endif
