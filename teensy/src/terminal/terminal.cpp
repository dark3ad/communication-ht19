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
#include <bsp.h>
#include <sdcard.h>
#include <unistd.h>
#include <stdlib.h>

static menu_t main_menu;
static menu_t temperature_menu;
static menu_t humidity_menu;
static menu_t light_menu;
static menu_t flow_meter_menu;
static menu_t moisture_menu;
static menu_t water_level_menu;
static menu_t logged_files_menu;
static menu_t calibration_menu;

static menu_t * current_menu;
static char input[READ_BUFFER_SIZE] = {0};

static void read_bsp_string(char * input)
{
  uint8_t index = 0;
  while(bsp_serial_available)
  {
    if(bsp_serial_available() > 0)
    {
      *(input + index) = bsp_serial_read();
    }
  }
}

static void calibration_menu_presenter(void * args)
{
  printf("%d) Temperature.\n", CALIBRATION_MENU_TEMPERATURE);
  printf("%d) Humidity.\n", CALIBRATION_MENU_HUMIDITY);
  printf("%d) Flow Meter.\n", CALIBRATION_MENU_FLOW_METER);
  printf("%d) Water Level.\n", CALIBRATION_MENU_WATER_LEVEL);
  printf("b) Back to main menu.\n");
  printf("q) Quit.\n");

}

static void calibration_menu_handler(void * args)
{
    uint8_t opt = strtol((const char *)args, NULL, 10);

    switch(opt){
        case CALIBRATION_MENU_TEMPERATURE:
            current_menu = &temperature_menu;
            break;
        case CALIBRATION_MENU_HUMIDITY:
            current_menu = &humidity_menu;
            break;
        case CALIBRATION_MENU_FLOW_METER:
            current_menu = &flow_meter_menu;
            break;
        case CALIBRATION_MENU_WATER_LEVEL:
            current_menu = &water_level_menu;
            break;
        default:
            printf("Nope");
            break;
    }
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
  uint8_t opt = strtol((const char *)args, NULL, 10);

  switch(opt){
      case WATER_LEVEL_MENU_TARGET_MIN:
          printf("Enter target min:");
          read_bsp_string(input);

          opt = strtol(input, NULL, 10);
          set_water_level_target_min(opt);
      break;
    case WATER_LEVEL_MENU_TARGET_MAX:
          printf("Enter target max:");
          read_bsp_string(input);

          opt = strtol(input, NULL, 10);
          set_water_level_target_max(opt);
    case WATER_LEVEL_MENU_CAL_MAX:
          printf("Enter calibration max:");
          read_bsp_string(input);

          opt = strtol(input, NULL, 10);
          set_water_level_calibration_max(opt);
      break;
    default:
      printf("Nope");
      break;
  }
}

static void water_level_menu_presenter(void * args)
{
  
}

static void moisture_menu_presenter(void * args)
{
    if(args == NULL)
        return;
}

static void moisture_menu_handler(void * args)
{
  uint8_t opt = strtol((const char *)args, NULL, 10);

  switch(opt){
      case MOIS_MENU_SET_TARGET_MIN:
          printf("Enter target min:");
          read_bsp_string(input);

          opt = strtol(input, NULL, 10);
          set_soil_moisture_target_min(opt);
      break;
    case MOIS_MENU_SET_TARGET_MAX:
          printf("Enter target max:");
          read_bsp_string(input);

          opt = strtol(input, NULL, 10);
          set_soil_moisture_target_max(opt);
      break;
    default:
      printf("Nope");
      break;
  }
}

static void flow_meter_menu_presenter(void * args)
{
  printf("%d) Set calibration max.\n", FLOW_MENU_SET_CALI_MAX);
}

static void flow_meter_menu_handler(void * args)
{
    uint16_t opt = strtol((const char *)args, NULL, 10);
    char input[5];

    switch(opt) 
    {
    case FLOW_MENU_SET_CALI_MAX:
        printf("Enter calibration max:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_flow_meter_calibration_max(opt);
        break;
    default:
        break;
    }
}

static void light_menu_handler(void * args)
{
    uint16_t opt = strtol((const char *)args, NULL, 10);
    char input[5];

    switch(opt) 
    {
    case LIGHT_MENU_TARGET_MIN:
        printf("Enter target min:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_light_intensity_target_min(opt);
        break;
    default:
        break;
    }
}

static void light_menu_presenter(void * args)
{

}

static void humidity_menu_handler(void * args)
{
    uint8_t opt = strtol((const char *)args, NULL, 10);
    char input[3];

    switch(opt) 
    {
    case HUM_MENU_SET_TARGET_MIN:
        printf("Enter target min:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_target_max(opt);
        break;
    case HUM_MENU_SET_TARGET_MAX:
        printf("Enter target max:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_target_min(opt);
        break;
    case HUM_MENU_SET_CALI_MIN:
        printf("Enter calibration min:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_humidity_calibration_min(opt);
        break;
    case HUM_MENU_SET_CALI_MAX:
        printf("Enter calibration max:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_humidity_calibration_max(opt);
        break;
    default:
        break;
    }
}

static void humidity_menu_presenter(void * args)
{
  printf("%d) Set target min.\n", HUM_MENU_SET_TARGET_MIN);
  printf("%d) Set target max.\n", HUM_MENU_SET_TARGET_MAX);
  printf("%d) Set calibration min.\n", HUM_MENU_SET_CALI_MIN);
  printf("%d) Set calibration max.\n", HUM_MENU_SET_CALI_MAX);
  printf("b) Return to calibration.\n");
  printf("q) Quit.\n");
  printf("> ");
}

static void temperature_menu_handler(void * args)
{
    uint8_t opt = strtol((const char *)args, NULL, 10);
    char input[3];

    switch(opt) 
    {
    case TEMP_MENU_SET_TARGET_MAX:
        printf("Enter target max:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_target_max(opt);
        break;
    case TEMP_MENU_SET_TARGET_MIN:
        printf("Enter target min:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_target_min(opt);
        break;
    case TEMP_MENU_SET_CALI_MIN:
        printf("Enter calibration min:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_calibration_min(opt);
        break;
    case TEMP_MENU_SET_CALI_MAX:
        printf("Enter calibration max:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_calibration_max(opt);
        break;
    default:
        break;
    }
}

static void temperature_menu_presenter(void * args)
{
  printf("%d) Set target min.\n", TEMP_MENU_SET_TARGET_MIN);
  printf("%d) Set target max.\n", TEMP_MENU_SET_TARGET_MAX);
  printf("%d) Set calibration min.\n", TEMP_MENU_SET_CALI_MIN);
  printf("%d) Set calibration max.\n", TEMP_MENU_SET_CALI_MAX);
  printf("b) Return to calibration.\n");
  printf("q) Quit.\n");
  printf("> ");
}

static void logged_files_menu_handler(void * args)
{
  char * name = (char *)args;
  filelist_t list = sdcard_get_files_list();
  char buffer[255] = {};
  for(uint8_t i = 0; i < 2; i++)
  {
    if(memcmp(list.logs[i], args, sizeof(char) * 2) == 0)
    {
      sdcard_read_file(list.logs[i], buffer, 13);
      printf("%s\n", buffer);
      return;
    }
  }
  printf("Could not find file");
}

static void logged_files_menu_presenter(void * args)
{
  filelist_t list = sdcard_get_files_list();
  for(uint8_t i = 0; i < 2; i++)
  {
    printf("%s\n", list.logs[i]);
  }
  printf("b) Return to main menu.\n");
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
  uint8_t opt = strtol((const char*)args, NULL, 10);

  switch(opt) {
    case MAIN_MENU_SNAPSHOT:
      get_snapshot(args);
      break;
    case MAIN_MENU_FILES:
      current_menu = &logged_files_menu;
      break;
    case MAIN_MENU_CALIBRATION:
      current_menu = &calibration_menu;
      break;
    default:
      break;
  }
}

int terminal_initialize(void)
{
  logged_files_menu = {"logged_files", logged_files_menu_presenter, logged_files_menu_handler};
  calibration_menu = {"calibration", calibration_menu_presenter, calibration_menu_handler};
  temperature_menu = {"temperature_menu", temperature_menu_presenter, temperature_menu_handler};
  humidity_menu = {"humidity_menu", humidity_menu_presenter, humidity_menu_handler};
  flow_meter_menu = {"flow_meter_menu", flow_meter_menu_presenter, flow_meter_menu_handler};
  current_menu = &main_menu;

  // Check the SD card status.
  if(get_sdcard_status() != OKAY)
  {
    // Then, turn it on, how hard can it be?
    // set_sdcard_status(OKAY);
  }
  
  // Everything is fine, you are good to go!
  return 0;
}

int terminal_run(void)
{
  while(true)
  {
    // Display the current menu.
    printf("---- THE GREENHOUSE ----\n");

    current_menu->presenter(NULL);

    if(bsp_serial_available())
    {
      read_bsp_string(input);
    }

    if(memcmp(input, "q", 1) == 0)
    {
      return 0;
    }
    current_menu->handler(input);
  }
  return 0;
}
