/**
 * @author      : jesper (jesper@jesper-HP-ENVY-Laptop-13-aq0xxx)
 * @file        : terminal
 * @created     : tisdag nov 10, 2020 13:28:56 CET
 */

#include <stdint.h>
#include <stdio.h>
#include <terminal.h>
#include <canbus.h>
#include <candata.h>
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
  bool (* setter_uint8)(uint8_t value);
  bool (* setter_uint16)(uint16_t value);
  const char * message;
};

static breadcrumbs_t breadcrumbs;
static canbus_data_t canbus_data;

static char input[READ_BUFFER_SIZE] = {0};

static void temperature_menu_presenter(void * args);
static void temperature_menu_handler(void * args);

static void humidity_menu_presenter(void * args);
static void humidity_menu_handler(void * args);

static void light_menu_presenter(void * args);
static void light_menu_handler(void * args);

static void moisture_menu_presenter(void * args);
static void moisture_menu_handler(void * args);

static void water_level_menu_presenter(void * args);
static void water_level_menu_handler(void * args);

static void breadcrumbs_enter(void (* h)(void *), void (* p)(void *));
static void breadcrumbs_leave();

static void update_canbus_uint8(void * args);
static void update_canbus_uint16(void * args);

static void target_bool_presenter(void * args)
{
  bsp_serial_write("Enter 0/1: ");
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

static void calibration_menu_presenter(void * args)
{
  bsp_serial_print(CALIBRATION_MENU_TEMPERATURE);
  bsp_serial_write(") Temperature.\n");
  bsp_serial_print(CALIBRATION_MENU_HUMIDITY);
  bsp_serial_write(") Humidity.\n");
  bsp_serial_print(CALIBRATION_MENU_LIGHT_INTENSITY);
  bsp_serial_write(") Light Intensity.\n");
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
    case CALIBRATION_MENU_SOIL_MOISTURE:
      breadcrumbs_enter(moisture_menu_handler, moisture_menu_presenter);
      break;
    case CALIBRATION_MENU_WATER_LEVEL:
      breadcrumbs_enter(water_level_menu_handler, water_level_menu_presenter);
      break;
    case CALIBRATION_MENU_LIGHT_INTENSITY:
      breadcrumbs_enter(light_menu_handler, light_menu_presenter);
      break;
    default:
      break;
  }
}

static void get_snapshot(void)
{
  data_t data = get_candata();
  bsp_serial_write("\nheater_status: ");
  bsp_serial_print(data.actuator.heater.status);

  bsp_serial_write("\nheater_warmness_overwritten: ");
  bsp_serial_print(data.actuator.heater.warmness.overwritten);

  bsp_serial_write("\nheater_warmness: ");
  bsp_serial_print(data.actuator.heater.warmness.value);

  bsp_serial_write("\nwater_valve_status: ");
  bsp_serial_print(data.actuator.water_valve.status);

  bsp_serial_write("\nwater_valve_state_overwritten: ");
  bsp_serial_print(data.actuator.water_valve.state.overwritten);

  bsp_serial_write("\nwater_valve_state: ");
  bsp_serial_print(data.actuator.water_valve.state.value);

  bsp_serial_write("\nfans_status: ");
  bsp_serial_print(data.actuator.fans.status);

  bsp_serial_write("\nfans_state_overwritten: ");
  bsp_serial_print(data.actuator.fans.state.overwritten);

  bsp_serial_write("\nfans_state: ");
  bsp_serial_print(data.actuator.fans.state.value);

  bsp_serial_write("\nwindow_status: ");
  bsp_serial_print(data.actuator.window.status);

  bsp_serial_write("\nwindow_state_overwritten: ");
  bsp_serial_print(data.actuator.window.state.overwritten);

  bsp_serial_write("\nwindow_state: ");
  bsp_serial_print(data.actuator.window.state.value);

  bsp_serial_write("\nlamp_status: ");
  bsp_serial_print(data.actuator.lamp.status);

  bsp_serial_write("\nlamp_shininess_overwritten: ");
  bsp_serial_print(data.actuator.lamp.shininess.overwritten);

  bsp_serial_write("\nlamp_shininess: ");
  bsp_serial_print(data.actuator.lamp.shininess.value);

  bsp_serial_write("\nwater_pump_status: ");
  bsp_serial_print(data.actuator.water_pump.status);

  bsp_serial_write("\nwater_pump_state_overwritten: ");
  bsp_serial_print(data.actuator.water_pump.state.overwritten);

  bsp_serial_write("\nwater_pump_state: ");
  bsp_serial_print(data.actuator.water_pump.state.value);

  bsp_serial_write("\nterminal_status: ");
  bsp_serial_print(data.communication.terminal.status);

  bsp_serial_write("\nesp32_status: ");
  bsp_serial_print(data.communication.esp32.status);

  bsp_serial_write("\nrtc_status: ");
  bsp_serial_print(data.communication.rtc.status);

  bsp_serial_write("\nsdcard_status: ");
  bsp_serial_print(data.communication.sdcard.status);

  bsp_serial_write("\nsdcard_free_space: ");
  bsp_serial_print(data.communication.sdcard.free_space);

  bsp_serial_write("\nlight_intensity_sensor_status: ");
  bsp_serial_print(data.sensor.light.status);

  bsp_serial_write("\nlight_intensity: ");
  bsp_serial_print(data.sensor.light.intensity);

  bsp_serial_write("\nlight_intensity_target_min: ");
  bsp_serial_print(data.sensor.light.target.valid);

  bsp_serial_write("\nlight_intensity_target_min: ");
  bsp_serial_print(data.sensor.light.target.value);

  bsp_serial_write("\nflow_meter_sensor_status: ");
  bsp_serial_print(data.sensor.flow_meter.status);

  bsp_serial_write("\nflow_rate: ");
  bsp_serial_print(data.sensor.flow_meter.value);

  bsp_serial_write("\ndht_sensor_status: ");
  bsp_serial_print(data.sensor.humidity.status);

  bsp_serial_write("\nhumidity_target_max: ");
  bsp_serial_print(data.sensor.humidity.target.max.valid);

  bsp_serial_write("\nhumidity_target_max: ");
  bsp_serial_print(data.sensor.humidity.target.max.value);

  bsp_serial_write("\nhumidity_target_min: ");
  bsp_serial_print(data.sensor.humidity.target.min.valid);

  bsp_serial_write("\nhumidity_target_min: ");
  bsp_serial_print(data.sensor.humidity.target.min.value);

  bsp_serial_write("\nhumidity: ");
  bsp_serial_print(data.sensor.humidity.value);

  bsp_serial_write("\nhumidity_calibration_max: ");
  bsp_serial_print(data.sensor.humidity.calibration.max.valid);

  bsp_serial_write("\nhumidity_calibration_max: ");
  bsp_serial_print(data.sensor.humidity.calibration.max.value);

  bsp_serial_write("\nhumidity_calibration_min: ");
  bsp_serial_print(data.sensor.humidity.calibration.min.valid);

  bsp_serial_write("\nhumidity_calibration_min: ");
  bsp_serial_print(data.sensor.humidity.calibration.min.value);

  bsp_serial_write("\nsoil_moisture_sensor_status: ");
  bsp_serial_print(data.sensor.moisture.status);

  bsp_serial_write("\nsoil_moisture_target_max: ");
  bsp_serial_print(data.sensor.moisture.target.max.valid);

  bsp_serial_write("\nsoil_moisture_target_max: ");
  bsp_serial_print(data.sensor.moisture.target.max.value);

  bsp_serial_write("\nsoil_moisture_target_min: ");
  bsp_serial_print(data.sensor.moisture.target.min.valid);

  bsp_serial_write("\nsoil_moisture_target_min: ");
  bsp_serial_print(data.sensor.moisture.target.min.value);

  bsp_serial_write("\nsoil_moisture: ");
  bsp_serial_print(data.sensor.moisture.value);

  bsp_serial_write("\nwater_level_sensor_status: ");
  bsp_serial_print(data.sensor.water_level.status);

  bsp_serial_write("\nwater_level_target_max: ");
  bsp_serial_print(data.sensor.water_level.target.max.valid);

  bsp_serial_write("\nwater_level_target_max: ");
  bsp_serial_print(data.sensor.water_level.target.max.value);

  bsp_serial_write("\nwater_level_target_min: ");
  bsp_serial_print(data.sensor.water_level.target.min.valid);

  bsp_serial_write("\nwater_level_target_min: ");
  bsp_serial_print(data.sensor.water_level.target.min.value);

  bsp_serial_write("\nwater_level: ");
  bsp_serial_print(data.sensor.water_level.value);

  bsp_serial_write("\ndht_sensor_status: ");
  bsp_serial_print(data.sensor.temperature.status);

  bsp_serial_write("\ntemperature_target_max: ");
  bsp_serial_print(data.sensor.temperature.target.max.valid);

  bsp_serial_write("\ntemperature_target_max: ");
  bsp_serial_print(data.sensor.temperature.target.max.value);

  bsp_serial_write("\ntemperature_target_min: ");
  bsp_serial_print(data.sensor.temperature.target.min.valid);

  bsp_serial_write("\ntemperature_target_min: ");
  bsp_serial_print(data.sensor.temperature.target.min.value);

  bsp_serial_write("\ntemperature: ");
  bsp_serial_print(data.sensor.temperature.value);

  bsp_serial_write("\ntemperature_calibration_max: ");
  bsp_serial_print(data.sensor.temperature.calibration.max.valid);

  bsp_serial_write("\ntemperature_calibration_max: ");
  bsp_serial_print(data.sensor.temperature.calibration.max.value);

  bsp_serial_write("\ntemperature_calibration_min: ");
  bsp_serial_print(data.sensor.temperature.calibration.min.valid);

  bsp_serial_write("\ntemperature_calibration_min: ");
  bsp_serial_print(data.sensor.temperature.calibration.min.value);

  bsp_serial_write("\nbuzzer_status: ");
  bsp_serial_print(data.hmi.buzzer.status);

  bsp_serial_write("\nkeypad_status: ");
  bsp_serial_print(data.hmi.keypad.status);

  bsp_serial_write("\nsystem_status: ");
  bsp_serial_print(data.hmi.system.status);

  bsp_serial_write("\nrgb_status: ");
  bsp_serial_print(data.hmi.rgb.status);

  bsp_serial_write("\ndisplay_status: ");
  bsp_serial_print(data.hmi.display.status);

  bsp_serial_write("\neeprom_status: ");
  bsp_serial_print(data.hmi.eeprom.status);
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
  bsp_serial_print(WATER_LEVEL_MENU_TARGET_MIN);
  bsp_serial_write(") Update water level target min.\n");
  bsp_serial_print(WATER_LEVEL_MENU_TARGET_MAX);
  bsp_serial_write(") Update water level target max.\n");
  bsp_serial_print(WATER_LEVEL_MENU_VOLUME_MAX);
  bsp_serial_write(") Update water volume target max.\n");
  bsp_serial_write("b) Back to calibration.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
}

static void moisture_menu_presenter(void * args)
{
  bsp_serial_print(MOIS_MENU_SET_TARGET_MIN);
  bsp_serial_write(") Update soil moisture sensor target min.\n");
  bsp_serial_print(MOIS_MENU_SET_TARGET_MAX);
  bsp_serial_write(") Update soil moisture sensor target max.\n");
  bsp_serial_write("b) Back to calibration.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
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
  bsp_serial_print(LIGHT_MENU_TARGET_MIN);
  bsp_serial_write(") Update light intensity target min.\n");
  bsp_serial_write("b) Back to calibration.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
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

static void humidity_menu_presenter(void * args)
{
  bsp_serial_print(HUM_MENU_SET_TARGET_MIN);
  bsp_serial_write(") Set target min.\n");
  bsp_serial_print(HUM_MENU_SET_TARGET_MAX);
  bsp_serial_write(") Set target max.\n");
  bsp_serial_print(HUM_MENU_SET_CALI_MIN);
  bsp_serial_write(") Set calibration min.\n");
  bsp_serial_print(HUM_MENU_SET_CALI_MAX);
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
  bsp_serial_write("---- THE GREENHOUSE ----\n");
  bsp_serial_print(MAIN_MENU_SNAPSHOT);
  bsp_serial_write(") Get snapshop of the system.\n");
  bsp_serial_print(MAIN_MENU_FILES);
  bsp_serial_write(") Get list of logged files.\n");
  bsp_serial_print(MAIN_MENU_CALIBRATION);
  bsp_serial_write(") Calibrate the system.\n");
  bsp_serial_print(MAIN_MENU_ACTUATORS);
  bsp_serial_write(") Set actuators.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
}

static void actuators_menu_handler(void * args)
{

  uint8_t opt = strtol((const char *)args, NULL, 10);

  switch(opt) 
  {
    case ACT_MENU_LAMP_SHININESS:
      canbus_data.setter_uint8 = set_lamp_shininess;
      canbus_data.message = "Updated lamp shininess: ";
      break;
    case ACT_MENU_HEAT_WARMNESS:
      canbus_data.setter_uint8 = set_heater_warmness;
      canbus_data.message = "Updated heater warmness: ";
      break;
    case ACT_MENU_PUMP_STATE:
      canbus_data.setter_uint8 = set_water_pump_state;
      canbus_data.message = "Updated water pump state: ";
      break;
    case ACT_MENU_VALVE_STATE:
      canbus_data.setter_uint8 = set_water_valve_state;
      canbus_data.message = "Updated water vale state: ";
      break;
    case ACT_MENU_WINDOW_STATE:
      canbus_data.setter_uint8 = set_window_state;
      canbus_data.message = "Updated window state: ";
    case ACT_MENU_FAN_STATE:
      canbus_data.setter_uint8 = set_fans_state;
      canbus_data.message = "Updated fan state: ";
    default:
      return;
  }
  breadcrumbs_enter(update_canbus_uint8, target_bool_presenter);
}

static void actuators_menu_presenter(void * args)
{
  bsp_serial_print(ACT_MENU_LAMP_SHININESS);
  bsp_serial_write(") Set lamp shininess.\n");
  bsp_serial_print(ACT_MENU_HEAT_WARMNESS);
  bsp_serial_write(") Set heater warmness.\n");
  bsp_serial_print(ACT_MENU_PUMP_STATE);
  bsp_serial_write(") Set water pump state.\n");
  bsp_serial_print(ACT_MENU_VALVE_STATE);
  bsp_serial_write(") Set valve state.\n");
  bsp_serial_print(ACT_MENU_WINDOW_STATE);
  bsp_serial_write(") Set window state.\n");
  bsp_serial_print(ACT_MENU_FAN_STATE);
  bsp_serial_write(") Set fan state.\n");
  bsp_serial_write("b) Back to main menu.\n");
  bsp_serial_write("q) Quit.\n");
  bsp_serial_write("> ");
}

static void main_menu_handler(void * args)
{
  uint8_t opt = strtol((const char*)args, NULL, 10);

  switch(opt) {
    case MAIN_MENU_SNAPSHOT:
      get_snapshot();
      break;
    case MAIN_MENU_FILES:
      breadcrumbs_enter(logged_files_menu_handler, logged_files_menu_presenter);
      break;
    case MAIN_MENU_CALIBRATION:
      breadcrumbs_enter(calibration_menu_handler, calibration_menu_presenter);
      break;
    case MAIN_MENU_ACTUATORS:
      breadcrumbs_enter(actuators_menu_handler, actuators_menu_presenter);
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
    if(input[0] == 'b')
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

void terminal_run(void)
{
  display();
  read();
  handle();
}
