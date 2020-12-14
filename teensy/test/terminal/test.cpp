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
#include<common.h>
#include<candata.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdint.h>

bool bsp_serial_write_called = false;
bool bsp_serial_read_called = false;
bool get_candata_called = false;
bool sdcard_get_files_list_called = false;
bool sdcard_read_file_called = false;

bool set_temperature_target_min_called = false;

const char * input;
uint8_t input_index = 0;

/********* sdcard  ********/
typedef struct
{
    uint8_t init_status;
    uint16_t free_space;
    uint8_t read_status;
    uint8_t delete_status;
    uint8_t create_status;
    uint8_t append_status;
} sdcard_test_t;

sdcard_test_t sdcard = {};

uint8_t sdcard_init()
{
    return OKAY;
}

void reset_globals()
{
  bsp_serial_write_called = false;
  bsp_serial_read_called = false;
  get_candata_called = false;
  sdcard_get_files_list_called = false;
  sdcard_read_file_called = false;
}

filelist_t sdcard_get_files_list(void) 
{
  sdcard_get_files_list_called = true;
  filelist_t new_filelist = {
    SUCCESS,
    {"01", "02"}
  };

  return new_filelist;
}

uint8_t sdcard_read_file(const char *file_name, char *buffer, uint16_t length)
{
  buffer = NULL;
  sdcard_read_file_called = true;
  return 1;
  /**
    if (sdcard.read_status != SUCCESS)
    {
    return sdcard.read_status;
    }

    static uint32_t position = 0xFFFFFFFFU;

    int fd = open(file_name, O_RDONLY);
    if (fd < 0)
    {
    sdcard.read_status = FILE_NOT_EXIST;
    }
    else
    {
    read(fd, buffer, length);

    close(fd);
    }

    return sdcard.read_status;
   **/
}
data_t get_candata() 
{
  data_t data;
  get_candata_called = true;

  return data;
}

uint8_t bsp_serial_available(void)
{
  return 1;
}

char bsp_serial_read(void)
{
  bsp_serial_read_called = true;
  return input[input_index++];
}

void bsp_serial_write(const char *msg)
{
  bsp_serial_write_called = true;
}

void setUp()
{

}

void tearDown()
{

}

void bsp_serial_begin()
{

}

void test_terminal_init()
{
    reset_globals();
    terminal_init();
}

void test_terminal_menu_main_random_input()
{
  reset_globals();
  terminal_init();

  input_index = 0;
  input = "abc<>45aeragsgds5678hjensam";

  TEST_ASSERT_EQUAL_UINT8(false, get_candata_called);

  for(uint8_t i = 0; i < 40; i++)
  {
    terminal_run();
  }

  TEST_ASSERT_EQUAL_UINT8(false, get_candata_called);
}

void test_terminal_get_candata()
{
  reset_globals();
  terminal_init();

  input_index = 0;
  input = "0\n";

  TEST_ASSERT_EQUAL_UINT8(false, get_candata_called);

  while(!get_candata_called)
  {
    terminal_run();
  }

  TEST_ASSERT_EQUAL_UINT8(true, get_candata_called);
}

void test_get_files_list()
{
  reset_globals();
  terminal_init();

  input_index = 0;
  input = "1\n";

  TEST_ASSERT_EQUAL_UINT8(false, sdcard_get_files_list_called);
  TEST_ASSERT_EQUAL_UINT8(false, get_candata_called);

  while(!sdcard_get_files_list_called)
  {
    terminal_run();
  }

  TEST_ASSERT_EQUAL_UINT8(false, get_candata_called);
  TEST_ASSERT_EQUAL_UINT8(true, sdcard_get_files_list_called);
}

void test_sdcard_read_file()
{
  reset_globals();
  terminal_init();

  input_index = 0;
  input = "1\n01\n";

  TEST_ASSERT_EQUAL_UINT8(false, sdcard_get_files_list_called);
  TEST_ASSERT_EQUAL_UINT8(false, get_candata_called);
  TEST_ASSERT_EQUAL_UINT8(false, sdcard_read_file_called);

  while(!sdcard_read_file_called)
  {
    terminal_run();
  }

  TEST_ASSERT_EQUAL_UINT8(false, get_candata_called);
  TEST_ASSERT_EQUAL_UINT8(true, sdcard_get_files_list_called);
  TEST_ASSERT_EQUAL_UINT8(true, sdcard_read_file_called);
}

void test_set_temperature_target_min()
{
  reset_globals();
  terminal_init();

  input_index = 0;
  input = "2\n0\n2 15\n";

  TEST_ASSERT_EQUAL_UINT8(false, set_temperature_target_min_called);

  while(input_index >= strlen(input))
  {
    terminal_run();
  }

  TEST_ASSERT_EQUAL_UINT8(15, get_test_temperature_target_min());
}

void get_terminal_text(char*, terminal_t)
{
  ;
}

void get_esp32_text(char*, esp32_t)
{
  ;
}
void get_sdcard_text(char*, sdcard_t)
{
  ;
}
void get_rtc_text(char*, rtc_t)
{
  ;
}
void get_heater_text(char*, heater_t)
{
  ;
}
void get_water_valve_text(char*, water_valve_t)
{
  ;
}
void get_fans_text(char*, fans_t)
{
  ;
}
void get_window_text(char*, window_t)
{
  ;
}
void get_lamp_text(char*, lamp_t)
{
  ;
}
void get_water_pump_text(char*, water_pump_t)
{
  ;
}
void get_light_text(char*, light_t)
{
  ;
}
void get_flow_meter_text(char*, flow_meter_t)
{
  ;
}
void get_humidity_text(char*, humidity_t)
{
  ;
}
void get_moisture_text(char*, moisture_t)
{
  ;
}
void get_water_level_text(char*, water_level_t)
{
  ;
}
void get_temperature_text(char*, temperature_t)
{
  ;
}
void get_buzzer_text(char*, buzzer_t)
{
  ;
}
void get_keypad_text(char*, keypad_t)
{
  ;
}
void get_system_text(char*, system_t)
{
  ;
}
void get_rgb_text(char*, rgb_t)
{
  ;
}
void get_display_text(char*, display_t)
{
  ;
}
void get_eeprom_text(char*, eeprom_t)
{
  ;
}

void run()
{
    RUN_TEST(test_terminal_menu_main_random_input);
    RUN_TEST(test_terminal_init);
    RUN_TEST(test_terminal_get_candata);
    RUN_TEST(test_get_files_list);
    RUN_TEST(test_sdcard_read_file);
    RUN_TEST(test_set_temperature_target_min);
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
