/**
 * @author      : jesper (jesper@jesper-HP-ENVY-Laptop-13-aq0xxx)
 * @file        : terminal
 * @created     : tisdag nov 10, 2020 13:28:56 CET
 */

#include <stdint.h>
#include <stdio.h>
#include <terminal.h>
#include <canbus.h>
#include <common.h>
#include <string.h>
#include <bsp_io.h>

#define READ_BUFFER_SIZE 64

#define MAIN_MENU_SNAPSHOT 0
#define MAIN_MENU_FILES 1
#define MAIN_MENU_CALIBRATION 2

struct menu_t {
  const char * label;
  void (* presenter)(void *);
  void (* handler)(void *);
};

static menu_t main_menu;
static menu_t temperature_menu;
static menu_t humidity_menu;
static menu_t light_menu;
static menu_t flow_meter_menu;
static menu_t moisture_menu;
static menu_t water_level_menu;
static menu_t logged_files_menu;

static menu_t * current_menu;
static char input[READ_BUFFER_SIZE] = {0};

static void load_calibrations(void * args)
{

}

static void get_snapshot(void * args)
{

}

static void load_logged_files(void * args)
{
  (void)args;

  printf("The Logged Files");
}

static void water_level_menu_handler(void * args)
{
  
}

static void water_level_menu_presenter(void * args)
{
  
}

static void moisture_menu_presenter(void * args)
{

}

static void moisture_menu_handler(void * args)
{

}

static void flow_meter_menu_presenter(void * args)
{

}

static void flow_meter_menu_handler(void * args)
{

}

static void light_menu_handler(void * args)
{

}

static void light_menu_presenter(void * args)
{

}

static void humidity_menu_handler(void * args)
{

}

static void humidity_menu_presenter(void * args)
{

}

static void temperature_menu_handler(void * args)
{

}

static void temperature_menu_presenter(void * args)
{

}

static void logged_files_menu_handler(void * args)
{

}

static void logged_files_menu_presenter(void * args)
{
  printf("%d) \n", MAIN_MENU_SNAPSHOT);
  printf("q) Quit.\n");
  printf("> ");

}

static void main_menu_presenter(void * args)
{
  printf("%d) Get snapshop of the system.\n", MAIN_MENU_SNAPSHOT);
  printf("%d) Get list of logged files.\n", MAIN_MENU_FILES);
  printf("%d) Calibrate the system.\n", MAIN_MENU_CALIBRATION);
  printf("q) Quit.\n");
  printf("> ");
}

static void main_menu_handler(void * args)
{
  int *opt = (int *)args;
  *opt = *opt - 0 - 48;

  switch(*opt) {
    case MAIN_MENU_SNAPSHOT:
      get_snapshot(args);
      break;
    case MAIN_MENU_FILES:
      printf("Hello?");
      current_menu = &logged_files_menu;
      break;
    case MAIN_MENU_CALIBRATION:
      load_calibrations(args);
      break;
    default:
      break;
  }
}

int terminal_initialize(void)
{

  main_menu = {"main_menu", main_menu_presenter, main_menu_handler};
  logged_files_menu = {"logged_files", logged_files_menu_presenter, logged_files_menu_handler};
  current_menu = &main_menu;

  // Check the CAN-bus status.
  // Todo: check the CAN-bus status.

  // Check the SD card status.
  /*
  if(get_sdcard_status() != OKAY)
  {
    // Then, turn it on, how hard can it be?
    set_sdcard_status(OKAY);
  }
  */
  
  // Everything is fine, you are good to go!
  return 0;
}

int terminal_run(void)
{
  while(memcmp(input, "q", 1) != 0)
  {
    // Display the current menu.
    uint8_t i = 0;
    printf("---- THE GREENHOUSE ----\n");

    current_menu->presenter(NULL);

    int char_count = 0;
    if(bsp_serial_available())
    {
      input[char_count] = bsp_serial_read();
    }

    if(memcmp(input, "q", 1) == 0)
    {
      return 0;
    }
    current_menu->handler(input);
  }
  return 0;
}
