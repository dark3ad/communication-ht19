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

bool allow_display = true;
bool allow_read = false;
bool allow_handle = false;

uint8_t input_index = 0;

static void (* current_handler)(void *);
static void (* current_presenter)(void *);

static char input[READ_BUFFER_SIZE] = {0};

static void read_bsp_string(char * input)
{
  char value = 0;
  if(bsp_serial_available() > 0)
  {
    value = bsp_serial_read();

    if(value == '\n')
    {
      allow_read = false;
      allow_handle = true;
      input_index = 0;
      return;
    }

    bsp_serial_print_char(value);
    *(input + input_index) = value;
    input_index += 1;
  }
}

static void calibration_menu_presenter(void * args)
{
  bsp_serial_print(CALIBRATION_MENU_TEMPERATURE);
  bsp_serial_write("%d) Temperature.\n");
  bsp_serial_print(CALIBRATION_MENU_HUMIDITY);
  bsp_serial_write("%d) Humidity.\n");
  bsp_serial_print(CALIBRATION_MENU_FLOW_METER);
  bsp_serial_write("%d) Flow Meter.\n");
  bsp_serial_print(CALIBRATION_MENU_WATER_LEVEL);
  bsp_serial_write("%d) Water Level.\n");
  bsp_serial_write("b) Back to main menu.\n");
  bsp_serial_write("q) Quit.\n");

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
            bsp_serial_write("Nope");
            break;
    }
}

static void get_snapshot(void * args)
{

}

static void load_logged_files(void * args)
{
  (void)args;

  bsp_serial_write("The Logged Files");
}

static void water_level_menu_handler(void * args)
{
  uint8_t opt = strtol((const char *)args, NULL, 10);

  switch(opt){
      case WATER_LEVEL_MENU_TARGET_MIN:
          bsp_serial_write("Enter target min:");
          read_bsp_string(input);

          opt = strtol(input, NULL, 10);
          set_water_level_target_min(opt);
      break;
    case WATER_LEVEL_MENU_TARGET_MAX:
          bsp_serial_write("Enter target max:");
          read_bsp_string(input);

          opt = strtol(input, NULL, 10);
          set_water_level_target_max(opt);
    case WATER_LEVEL_MENU_CAL_MAX:
          bsp_serial_write("Enter calibration max:");
          read_bsp_string(input);

          opt = strtol(input, NULL, 10);
          //set_water_level_calibration_max(opt);
      break;
    default:
      bsp_serial_write("Nope");
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
          bsp_serial_write("Enter target min:");
          read_bsp_string(input);

          opt = strtol(input, NULL, 10);
          set_soil_moisture_target_min(opt);
      break;
    case MOIS_MENU_SET_TARGET_MAX:
          bsp_serial_write("Enter target max:");
          read_bsp_string(input);

          opt = strtol(input, NULL, 10);
          set_soil_moisture_target_max(opt);
      break;
    default:
      bsp_serial_write("Nope");
      break;
  }
}

static void flow_meter_menu_presenter(void * args)
{
  bsp_serial_write(FLOW_MENU_SET_CALI_MAX);
  bsp_serial_write("%d) Set calibration max.\n");
}

static void flow_meter_menu_handler(void * args)
{
    uint16_t opt = strtol((const char *)args, NULL, 10);
    char input[5];

    switch(opt) 
    {
    case FLOW_MENU_SET_CALI_MAX:
        bsp_serial_write("Enter calibration max:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        //set_flow_meter_calibration_max(opt);
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
        bsp_serial_write("Enter target min:");
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
        bsp_serial_write("Enter target min:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_target_max(opt);
        break;
    case HUM_MENU_SET_TARGET_MAX:
        bsp_serial_write("Enter target max:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_target_min(opt);
        break;
    case HUM_MENU_SET_CALI_MIN:
        bsp_serial_write("Enter calibration min:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_humidity_calibration_min(opt);
        break;
    case HUM_MENU_SET_CALI_MAX:
        bsp_serial_write("Enter calibration max:");
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
  bsp_serial_write(HUM_MENU_SET_TARGET_MIN);
  bsp_serial_write(") Set target min.\n");
  bsp_serial_write(HUM_MENU_SET_TARGET_MAX);
  bsp_serial_write(") Set target max.\n");
  bsp_serial_write(HUM_MENU_SET_CALI_MIN);
  bsp_serial_write(") Set calibration min.\n");
  bsp_serial_write(HUM_MENU_SET_CALI_MAX);
  bsp_serial_write(") Set calibration max.\n");
  bsp_serial_write("b) Return to calibration.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
}

static void update_temperature_target_max(void * args)
{
  uint8_t opt = strtol((const char *)args, NULL, 10);
  set_temperature_target_max(opt);
}

static void temperature_menu_handler(void * args)
{
    uint8_t opt = strtol((const char *)args, NULL, 10);
    char input[3];

    switch(opt) 
    {
    case TEMP_MENU_SET_TARGET_MAX:
        bsp_serial_write("Enter target max:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_target_max(opt);
        break;
    case TEMP_MENU_SET_TARGET_MIN:
        bsp_serial_write("Enter target min:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_target_min(opt);
        break;
    case TEMP_MENU_SET_CALI_MIN:
        bsp_serial_write("Enter calibration min:");
        read_bsp_string(input);

        opt = strtol(input, NULL, 10);
        set_temperature_calibration_min(opt);
        break;
    case TEMP_MENU_SET_CALI_MAX:
        bsp_serial_write("Enter calibration max:");
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
  bsp_serial_print(TEMP_MENU_SET_TARGET_MIN);
  bsp_serial_write(") Set target min.\n");
  bsp_serial_print(TEMP_MENU_SET_TARGET_MAX);
  bsp_serial_write(") Set calibration min.\n");
  bsp_serial_print(TEMP_MENU_SET_CALI_MIN);
  bsp_serial_write(") Set calibration min.\n");
  bsp_serial_print(TEMP_MENU_SET_CALI_MAX);
  bsp_serial_write(") Set calibration max.\n");
  bsp_serial_write("b) Return to calibration.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
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
      bsp_serial_write(buffer);
      bsp_serial_write("\n");
      return;
    }
  }
  bsp_serial_write("Could not find file");
}

static void logged_files_menu_presenter(void * args)
{
  filelist_t list = sdcard_get_files_list();
  for(uint8_t i = 0; i < 2; i++)
  {
    bsp_serial_write(list.logs[i]);
    bsp_serial_write("\n");
  }
  bsp_serial_write("b) Return to main menu.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
}

static void main_menu_presenter(void * args)
{
  bsp_serial_print(MAIN_MENU_SNAPSHOT);
  bsp_serial_write(") Get snapshop of the system.\n");
  bsp_serial_print(MAIN_MENU_FILES);
  bsp_serial_write(") Get list of logged files.\n");
  bsp_serial_print(MAIN_MENU_CALIBRATION);
  bsp_serial_write(") Calibrate the system.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
}

static void main_menu_handler(void * args)
{
  uint8_t opt = strtol((const char*)args, NULL, 10);

  switch(opt) {
    case MAIN_MENU_SNAPSHOT:
      break;
    case MAIN_MENU_FILES:
      current_handler = &logged_files_menu_handler;
      current_presenter = &logged_files_menu_presenter;
      break;
    case MAIN_MENU_CALIBRATION:
      current_handler = &calibration_menu_handler;
      current_presenter = &calibration_menu_presenter;
      break;
    default:
      break;
  }
}

int terminal_initialize(void)
{
  current_handler = &main_menu_handler;
  current_presenter = &main_menu_presenter;

  // Check the SD card status.
  if(get_sdcard_status() != OKAY)
  {
    // Then, turn it on, how hard can it be?
    // set_sdcard_status(OKAY);
  }
  
  // Everything is fine, you are good to go!
  return 0;
}

static void display()
{
  if(allow_display == true)
  {
    // Display the current menu.
    bsp_serial_write("---- THE GREENHOUSE ---- v0.3\n");

    current_presenter(NULL);
    allow_display = false;
    allow_read = true;
  }
}

static void read()
{
  if(allow_read == true)
  {
    read_bsp_string(input);
  }
}

static void handle()
{
  if(allow_handle == true)
  {
    current_handler(input);

    allow_handle = false;
    allow_display = true;
  }
}

int terminal_run(void)
{
  display();
  read();
  handle();
}
