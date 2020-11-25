#ifdef TARGET
#include<Arduino.h>
#endif

#include<unity.h>
#include<bsp_io.h>
#include<sdcard.h>
#include<terminal.h>
#include<stdio.h>
#include<string.h>

void set_soil_moisture_target_max(uint8_t value)
{
    printf("Soil Moisture Target Max updated: %d.\n", value);
}

void set_water_level_calibration_max(uint8_t value)
{
    printf("Water Level Calibration Max updated: %d.\n", value);
}

void set_water_level_target_max(uint8_t value)
{
    printf("Water Level Target Max updated: %d.\n", value);
}

void set_water_level_target_min(uint8_t value)
{
    printf("Water Level Target Min updated: %d.\n", value);
}

void set_soil_moisture_target_min(uint8_t value)
{
    printf("\nSoil Moisture Target Min updated: %d.\n", value);
}

void set_light_intensity_target_min(uint8_t value)
{
    printf("\nLight Intensity Min updated: %d.\n", value);
}

void set_flow_meter_calibration_max(uint16_t value)
{
    printf("\nFlow Meter Calibration Max updated: %d.\n", value);
}

void set_humidity_target_min(uint8_t value)
{
    printf("\nHumidity Target Min updated: %d.\n", value);
}

void set_humidity_target_max(uint8_t value)
{
    printf("\nHumidity Target Max updated: %d.\n", value);
}

void set_humidity_calibration_min(uint8_t value)
{
    printf("\nHumidity Calibration Min updated: %d.\n", value);
}

void set_humidity_calibration_max(uint8_t value)
{
    printf("\nHumidity Calibration Max updated: %d.\n", value);
}

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
 ; 
}

void test_menu_calibration_flow_target_min(void)
{
    uint8_t value = 0;
    printf("%s", flow_meter_menu.label);
    //flow_meter_menu.presenter(NULL);
}

void setUp()
{

}

void tearDown()
{

}


void run()
{
    terminal_initialize();

    test_menu_calibration_flow_target_min();
}

#ifdef TARGET
void loop()
{
}

void setup()
{
  delay(3000);
  UNITY_BEGIN();
  run();
  UNITY_END();
}
#else
int main()
{
  UNITY_BEGIN();
  run();
  return UNITY_END();
}
#endif
