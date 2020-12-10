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

char input;

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

filelist_t sdcard_get_files_list(void) 
{
  filelist_t new_filelist = {
    SUCCESS,
    {"01", "02"}
  };

  return new_filelist;
}

uint8_t sdcard_read_file(const char *file_name, char *buffer, uint16_t length)
{
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
  return input;
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
    terminal_init();
}

void test_terminal_get_candata()
{
  terminal_init();
  uint8_t index = 0;
  const char * inputs = "0\n";

  input = inputs[0];

  while(!get_candata_called)
  {
    if(bsp_serial_write_called)
    {
      index += 1;
      input = inputs[index];
    }
    terminal_run();
  }

  TEST_ASSERT_EQUAL_UINT8(true, get_candata_called);
}


void run()
{
    RUN_TEST(test_terminal_init);
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
