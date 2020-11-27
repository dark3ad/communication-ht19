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

struct breadcrumbs_t {
  void (*presenters[5])(void *);
  void (*handlers[5])(void *);
  uint8_t level;
};

struct canbus_data_t {
  uint8_t (* setter_uint8)(uint8_t value);
  uint8_t (* setter_uint16)(uint16_t value);
  char * message;
};

static breadcrumbs_t breadcrumbs;
static canbus_data_t canbus_data;

static char input[READ_BUFFER_SIZE] = {0};

static void temperature_menu_presenter(void * args);
static void temperature_menu_handler(void * args);
static void breadcrumbs_enter(void (* h)(void *), void (* p)(void *));
static void breadcrumbs_leave();

static void calibration_menu_presenter(void * args)
{
  bsp_serial_print(CALIBRATION_MENU_TEMPERATURE);
  bsp_serial_write(") Temperature.\n");
  bsp_serial_print(CALIBRATION_MENU_HUMIDITY);
  bsp_serial_write(") Humidity.\n");
  bsp_serial_print(CALIBRATION_MENU_FLOW_METER);
  bsp_serial_write(") Flow Meter.\n");
  bsp_serial_print(CALIBRATION_MENU_WATER_LEVEL);
  bsp_serial_write(") Water Level.\n");
  bsp_serial_write("b) Back to main menu.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
}

static void calibration_menu_handler(void * args)
{
  uint8_t opt = strtol((const char *)args, NULL, 10);
  switch(opt){
    case CALIBRATION_MENU_TEMPERATURE:
      breadcrumbs_enter(temperature_menu_handler, temperature_menu_presenter);
      break;
    case CALIBRATION_MENU_HUMIDITY:
      breadcrumbs_enter(humidity_menu_handler, humidity_menu_presenter);
      break;
    case CALIBRATION_MENU_FLOW_METER:
      break;
    case CALIBRATION_MENU_WATER_LEVEL:
      breadcrumbs_enter(water_level_menu_handler, water_level_menu_presenter);
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
      canbus_data.setter_uint8 = set_water_level_target_min;
      canbus_data.message = "Update water level target min: ";
      breadcrumbs_enter(update_canbus_uint8, target_min_presenter);
      break;
    case WATER_LEVEL_MENU_TARGET_MAX:
      canbus_data.setter_uint8 = set_water_level_target_max;
      canbus_data.message = "Update water level target max: ";
      breadcrumbs_enter(update_canbus_uint8, target_max_presenter);
      break;
    case WATER_LEVEL_MENU_VOLUME_MAX:
      canbus_data.setter_uint16 = set_water_volume_target_max;
      canbus_data.message = "Update water volume target max: ";
      breadcrumbs_enter(update_canbus_uint16, target_max_presenter);
      break;
    default:
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
	canbus_data.setter_uint8 = set_soil_moisture_target_min;
	canbus_data.message = "Update soil moisture target min: ";
	breadcrumbs_enter(update_canbus_uint8, target_min_presenter);
      break;
    case MOIS_MENU_SET_TARGET_MAX:
	canbus_data.setter_uint8 = set_soil_moisture_target_max;
	canbus_data.message = "Update soil moisture target max: ";
	breadcrumbs_enter(update_canbus_uint8, target_max_presenter);
      break;
    default:
      break;
  }
}

static void light_menu_handler(void * args)
{
    uint8_t opt = strtol((const char *)args, NULL, 10);
    switch(opt) 
    {
    case LIGHT_MENU_TARGET_MIN:
	canbus_data.setter_uint8 = set_light_intensity_target_min;
	canbus_data.message = "Update light intensity: ";
	breadcrumbs_enter(update_canbus_uint8, target_min_presenter);
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
    switch(opt) 
    {
    case HUM_MENU_SET_TARGET_MIN:
	canbus_data.setter_uint8 = set_humidity_target_min;
	canbus_data.message = "Update humidity target min: ";
	breadcrumbs_enter(update_canbus_uint8, target_min_presenter);
        break;
    case HUM_MENU_SET_TARGET_MAX:
	canbus_data.setter_uint8 = set_humidity_target_max;
	canbus_data.message = "Update humidity target max: ";
	breadcrumbs_enter(update_canbus_uint8, target_max_presenter);
        break;
    case HUM_MENU_SET_CALI_MIN:
	canbus_data.setter_uint8 = set_humidity_calibration_min;
	canbus_data.message = "Update humidity calibration min: ";
	breadcrumbs_enter(update_canbus_uint8, calibration_min_presenter);
        break;
    case HUM_MENU_SET_CALI_MAX:
	canbus_data.setter_uint8 = set_humidity_calibration_max;
	canbus_data.message = "Update humidity calibration max: ";
	breadcrumbs_enter(update_canbus_uint8, calibration_max_presenter);
        break;
    default:
        break;
    }
}

static void target_max_presenter(void * args)
{
  bsp_serial_write("Enter target max: ");
}

static void target_min_presenter(void * args)
{
  bsp_serial_write("Enter target min: ");
}

static void calibration_min_presenter(void * args)
{
  bsp_serial_write("Enter calibration min: ");
}

static void calibration_max_presenter(void * args)
{
  bsp_serial_write("Enter calibration max: ");
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

static void update_canbus_uint16(void * args)
{
  uint16_t opt = strtol((const char *)args, NULL, 10);

  if(canbus_data.setter_uint16(opt) == OKAY)
  {
    bsp_serial_write(canbus_data.message);
    bsp_serial_print(opt);
    bsp_serial_write("\n");
    breadcrumbs_leave();
  }
  else 
  {
    bsp_serial_write("Error: Failed to update.\n");
  }
}

static void update_canbus_uint8(void * args)
{
  uint8_t opt = strtol((const char *)args, NULL, 10);

  if(canbus_data.setter_uint8(opt) == OKAY)
  {
    bsp_serial_write(canbus_data.message);
    bsp_serial_print(opt);
    bsp_serial_write("\n");
    breadcrumbs_leave();
  }
  else 
  {
    bsp_serial_write("Error: Failed to update.\n");
  }
}

static void temperature_menu_handler(void * args)
{
    uint8_t opt = strtol((const char *)args, NULL, 10);

    switch(opt) 
    {
      case TEMP_MENU_SET_TARGET_MIN:
	canbus_data.setter_uint8 = set_temperature_target_min;
	canbus_data.message = "Update temperature target min: ";
	breadcrumbs_enter(update_canbus_uint8, target_min_presenter);
	break;
      case TEMP_MENU_SET_TARGET_MAX:
	canbus_data.setter_uint8 = set_temperature_target_max;
	canbus_data.message = "Update temperature target max: ";
	breadcrumbs_enter(update_canbus_uint8, target_max_presenter);
	break;
      case TEMP_MENU_SET_CALI_MIN:
	canbus_data.setter_uint8 = set_temperature_calibration_min;
	canbus_data.message = "Update temperature calibration min: ";
	breadcrumbs_enter(update_canbus_uint8, calibration_min_presenter);
	break;
      case TEMP_MENU_SET_CALI_MAX:
	canbus_data.setter_uint8 = set_temperature_calibration_max;
	canbus_data.message = "Update temperature calibration max: ";
	breadcrumbs_enter(update_canbus_uint8, calibration_max_presenter);
	break;
      default:
	return;
}

static void temperature_menu_presenter(void * args)
{
  bsp_serial_print(TEMP_MENU_SET_TARGET_MIN);
  bsp_serial_write(") Set target min.\n");
  bsp_serial_print(TEMP_MENU_SET_TARGET_MAX);
  bsp_serial_write(") Set target max.\n");
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
  bsp_serial_write("---- THE GREENHOUSE ---- v0.3\n");
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
      breadcrumbs_enter(&logged_files_menu_handler, &logged_files_menu_presenter);
      break;
    case MAIN_MENU_CALIBRATION:
      breadcrumbs_enter(&calibration_menu_handler, &calibration_menu_presenter);
      break;
    default:
      break;
  }
}

int terminal_initialize(void)
{
  breadcrumbs.level = 0;
  breadcrumbs.presenters[0] = main_menu_presenter;
  breadcrumbs.handlers[0] = main_menu_handler;

  // Check the SD card status.
  if(get_sdcard_status() != OKAY)
  {
    // Then, turn it on, how hard can it be?
    // set_sdcard_status(OKAY);
  }
  
  // Everything is fine, you are good to go!
  return 0;
}

static void breadcrumbs_enter(void (* h)(void *), void (* p)(void *))
{
  breadcrumbs.level += 1;

  breadcrumbs.handlers[breadcrumbs.level] = h;
  breadcrumbs.presenters[breadcrumbs.level] = p;
}

static void breadcrumbs_leave()
{
  breadcrumbs.level -= 1;
}

static void display()
{
  if(allow_display == true)
  {
    // Display the current menu.
    bsp_serial_write("---------------\n");
    breadcrumbs.presenters[breadcrumbs.level](NULL);
    allow_display = false;
    allow_read = true;
  }
}

static void read()
{
  if(allow_read == true)
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
}

static void handle()
{
  if(allow_handle == true)
  {
    if(input[0] == 'r')
    {
      breadcrumbs_leave();
      allow_handle = false;
      allow_display = true;
      return;
    }

    if(input[0] == 'q')
    {
      allow_handle = false;
      allow_display = false;
      allow_read = false;
      return;
    }

    breadcrumbs.handlers[breadcrumbs.level](input);

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
