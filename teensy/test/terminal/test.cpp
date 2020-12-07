#ifdef TARGET
#include<Arduino.h>
#endif

#include<unity.h>
#include<terminal.h>
#include<canbus.h>
#include<sdcard.h>
#include<terminal.h>
#include<stdio.h>
#include<string.h>

extern canbus_data_t canbus_data;

filelist_t sdcard_get_files_list(void) 
{
  filelist_t new_filelist = {
    SUCCESS,
    {"F1", "F2"}
  };
  return new_filelist;
}

uint8_t sdcard_read_file(const char *file_name, char *buffer, uint16_t length)
{
  return 0;
}

uint8_t bsp_serial_available(void)
{
  return 1;
}

char bsp_serial_read(void)
{
  return 0;
}

void bsp_serial_write(const char *msg)
{
  ;
}

void test_update_water_level_target_max(void)
{
  const char * value = "15";

  canbus_data.setter_uint8 = set_water_level_target_max;
  update_canbus_uint8((char *)value);

  TEST_ASSERT_EQUAL(15, get_test_water_level_target_max());
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
    RUN_TEST(test_update_water_level_target_max);
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
