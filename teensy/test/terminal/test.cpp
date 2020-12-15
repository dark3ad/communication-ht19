#ifdef TARGET
#include<Arduino.h>
#endif

#include<unity.h>
#include<terminal.h>
#include<sdcard.h>
#include<terminal.h>
#include<stdio.h>
#include<string.h>
#include<common.h>
#include<candata.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdint.h>

#define TERMINAL_INPUT_MAX 128

char terminal_input[TERMINAL_INPUT_MAX] = {};

/********* bsp  ********/
typedef struct
{
    uint8_t dtr;
    uint8_t available;

    const char * buffer;
    uint8_t buffer_index;

    char * write_output[2];
} bsp_test_t;

bsp_test_t bsp = {};

/********* sdcard  ********/
typedef struct
{
    uint8_t init_status;
    uint8_t read_status;
    filelist_t filelist;
} sdcard_test_t;

sdcard_test_t sdcard = {};

int bsp_serial_dtr()
{
    return bsp.dtr;
}

uint8_t sdcard_init()
{
    return sdcard.init_status;
}

/********* sdcard test double ********/
static bool check_file_name(const char *name)
{
    bool valid = false;
    if (!strcmp(name, ERROR_LOG))
    {
        valid = true;
    }
    else
    {
        for (uint8_t i = 1; i <= DAYS; i++)
        {
            char temp[FILE_LENGTH] = {};
            sprintf(temp, "%02d", i);
            if (!strcmp(temp, name))
            {
                valid = true;
                break;
            }
        }
    }
    return valid;
}


uint8_t sdcard_read_file(const char *file_name, char *buffer, uint16_t length)
{
    if (sdcard.read_status != SUCCESS)
    {
	return sdcard.read_status;
    }

    static uint32_t position = 0;

    if (!check_file_name(file_name))
    {
	position = 0;
	sdcard.read_status = INVALID_FILE_NAME;
	return sdcard.read_status;
    }

#ifdef TARGET

#else
    int fd = open(file_name, O_RDONLY);
    if (fd < 0)
    {
	position = 0;
	sdcard.read_status = FILE_NOT_EXIST;
    }
    else
    {
	memset(buffer, 0, length);
	lseek(fd, position, SEEK_SET);

	length--;
	ssize_t size = read(fd, buffer, length);

	if (size < 0)
	{
	    sdcard.read_status = READ_FILE_ERROR;
	}
	else if (size < length)
	{
	    position = 0;
	}
	else
	{
	    position += size;
	}
	close(fd);
    }
#endif

    return sdcard.read_status;
}

filelist_t sdcard_get_files_list(void)
{
#ifdef TARGET

#else
    return sdcard.filelist;
#endif
}

uint8_t bsp_serial_available(void)
{
    return bsp.available;
}

char bsp_serial_read(void)
{
    return bsp.buffer[bsp.buffer_index++];
}

void bsp_serial_write(const char *msg)
{
   memcpy(bsp.write_output, msg, 1);
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

void test_terminal_menu_random_one_valid_input()
{
  terminal_init();

  bsp.buffer_index = 0;
  bsp.buffer = "abc<>45aeragsgds15678hjensam";

  while(memcmp(bsp.write_output, "1", 1) == 0)
  {
    terminal_run();
  }

  TEST_ASSERT_EQUAL_STRING("1", bsp.write_output);
}

void test_terminal_menu_random_no_valid_input()
{
  terminal_init();

  bsp.buffer_index = 0;
  bsp.buffer = "abc<>45aeragsgds5678hjensam";

  for(uint8_t i = 0; i < 40; i++)
  {
    terminal_run();
  }

  TEST_ASSERT_EQUAL_STRING(NULL, bsp.write_output);
}

void test_terminal_get_candata()
{
  terminal_init();

  bsp.buffer_index = 0;
  bsp.buffer = "0\n";

/*
  while(!get_candata_called)
  {
    terminal_run();
  }
*/
}

void test_get_files_list()
{
  terminal_init();

  bsp.buffer_index = 0;
  bsp.buffer = "1\n";
/*
  while(!sdcard_get_files_list_called)
  {
    terminal_run();
  }
*/
}

void test_sdcard_read_file()
{
  terminal_init();

  bsp.buffer_index = 0;
  bsp.buffer = "1\n01\n";
/*
  while(!sdcard_read_file_called)
  {
    terminal_run();
  }
*/
}

void run()
{
    RUN_TEST(test_terminal_menu_random_no_valid_input);
    RUN_TEST(test_terminal_menu_random_one_valid_input);
    RUN_TEST(test_terminal_get_candata);
    RUN_TEST(test_get_files_list);
    RUN_TEST(test_sdcard_read_file);
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
