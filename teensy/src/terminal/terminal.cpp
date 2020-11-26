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
  while(bsp_serial_available())
  {
    if(bsp_serial_available() > 0)
    {
      *(input + index) = bsp_serial_read();
      index += 1;
    }
    else {
      return;
    }
  }
}

static void calibration_menu_presenter(void * args)
{
  bsp_serial_write(CALIBRATION_MENU_TEMPERATURE);
  bsp_serial_write("%d) Temperature.\n");
  bsp_serial_write(CALIBRATION_MENU_HUMIDITY);
  bsp_serial_write("%d) Humidity.\n");
  bsp_serial_write(CALIBRATION_MENU_FLOW_METER);
  bsp_serial_write("%d) Flow Meter.\n");
  bsp_serial_write(CALIBRATION_MENU_WATER_LEVEL);
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
  bsp_serial_write("%d) Set target min.\n");
  bsp_serial_write(HUM_MENU_SET_TARGET_MAX);
  bsp_serial_write("%d) Set target max.\n");
  bsp_serial_write(HUM_MENU_SET_CALI_MIN);
  bsp_serial_write("%d) Set calibration min.\n");
  bsp_serial_write(HUM_MENU_SET_CALI_MAX);
  bsp_serial_write("%d) Set calibration max.\n");
  bsp_serial_write("b) Return to calibration.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
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
  bsp_serial_write(TEMP_MENU_SET_TARGET_MIN);
  bsp_serial_write(") Set target min.\n");
  bsp_serial_write(TEMP_MENU_SET_TARGET_MAX);
  bsp_serial_write(") Set calibration min.\n");
  bsp_serial_write(TEMP_MENU_SET_CALI_MIN);
  bsp_serial_write(") Set calibration min.\n");
  bsp_serial_write(TEMP_MENU_SET_CALI_MAX);
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
  bsp_serial_write(MAIN_MENU_SNAPSHOT);
  bsp_serial_write(") Get snapshop of the system.\n");
  bsp_serial_write(MAIN_MENU_FILES);
  bsp_serial_write(") Get list of logged files.\n");
  bsp_serial_write(MAIN_MENU_CALIBRATION);
  bsp_serial_write(") Calibrate the system.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
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
  main_menu = {"main_menu", main_menu_presenter, main_menu_handler};
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
  // Display the current menu.
  bsp_serial_write("---- THE GREENHOUSE ---- v0.3\n");

  current_menu->presenter(NULL);

  bsp_serial_print(bsp_serial_available());
  bsp_serial_write("\n");
  delay(500);
  read_bsp_string(input);

  if(memcmp(input, "q", 1) == 0)
  {
    return 0;
  }

  current_menu->handler(input);
}
