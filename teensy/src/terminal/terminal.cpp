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

#define READ_BUFFER_SIZE 64

#define MAIN_MENU_SNAPSHOT 0
#define MAIN_MENU_FILES 1
#define MAIN_MENU_CALIBRATION 2

#define CALIBRATION_MENU_TEMPERATURE 0
#define CALIBRATION_MENU_HUMIDITY 1
#define CALIBRATION_MENU_LIGHT_INTENSITY 2
#define CALIBRATION_MENU_WATER_LEVEL 3
#define CALIBRATION_MENU_SOIL_MOISTURE 4

#define TEMP_MENU_SET_TARGET_MIN 0
#define TEMP_MENU_SET_TARGET_MAX 1
#define TEMP_MENU_SET_CALI_MIN 2
#define TEMP_MENU_SET_CALI_MAX 3

#define HUM_MENU_SET_TARGET_MIN 0
#define HUM_MENU_SET_TARGET_MAX 1
#define HUM_MENU_SET_CALI_MIN 2
#define HUM_MENU_SET_CALI_MAX 3

#define MOIS_MENU_SET_TARGET_MIN 0
#define MOIS_MENU_SET_TARGET_MAX 1

#define WATER_LEVEL_MENU_TARGET_MIN 0
#define WATER_LEVEL_MENU_TARGET_MAX 1

#define LIGHT_MENU_TARGET_MIN 0

bool allow_display = true;
bool allow_read = false;
bool allow_handle = false;

char allowed_chars[] = "b0123456789-\n\0";

uint8_t input_index = 0;

struct breadcrumbs_t {
    void (*presenters[5])(void);
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

static void temperature_menu_presenter(void);
static void temperature_menu_handler(void * args);

static void humidity_menu_presenter(void);
static void humidity_menu_handler(void * args);

static void light_menu_presenter(void);
static void light_menu_handler(void * args);

static void moisture_menu_presenter(void);
static void moisture_menu_handler(void * args);

static void water_level_menu_presenter(void);
static void water_level_menu_handler(void * args);

static void breadcrumbs_enter(void (* h)(void *), void (* p)(void));
static void breadcrumbs_leave();

static void update_canbus_uint8(void * args);

static void target_max_presenter(void)
{
    bsp_serial_write("Enter target max: ");
}

static void target_min_presenter(void)
{
    bsp_serial_write("Enter target min: ");
}

static void calibration_min_presenter(void)
{
    bsp_serial_write("Enter calibration min: ");
}

static void calibration_max_presenter(void)
{
    bsp_serial_write("Enter calibration max: ");
}

static void calibration_menu_presenter(void)
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
    char buffer[4096] = {};

    sprintf(buffer + strlen(buffer), "\n\n[%s] ================================\n", data.communication.rtc.datetime);

    get_terminal_text(buffer + strlen(buffer), data.communication.terminal);
    get_esp32_text(buffer + strlen(buffer), data.communication.esp32);
    get_sdcard_text(buffer + strlen(buffer), data.communication.sdcard);
    get_rtc_text(buffer + strlen(buffer), data.communication.rtc);

    sprintf(buffer + strlen(buffer), "\n================================\n");

    get_heater_text(buffer + strlen(buffer), data.actuator.heater);
    get_water_valve_text(buffer + strlen(buffer), data.actuator.water_valve);
    get_fans_text(buffer + strlen(buffer), data.actuator.fans);
    get_window_text(buffer + strlen(buffer), data.actuator.window);
    get_lamp_text(buffer + strlen(buffer), data.actuator.lamp);
    get_water_pump_text(buffer + strlen(buffer), data.actuator.water_pump);

    sprintf(buffer + strlen(buffer), "\n================================\n");

    get_light_text(buffer + strlen(buffer), data.sensor.light);
    get_flow_meter_text(buffer + strlen(buffer), data.sensor.flow_meter);
    get_humidity_text(buffer + strlen(buffer), data.sensor.humidity);
    get_moisture_text(buffer + strlen(buffer), data.sensor.moisture);
    get_water_level_text(buffer + strlen(buffer), data.sensor.water_level);
    get_temperature_text(buffer + strlen(buffer), data.sensor.temperature);

    sprintf(buffer + strlen(buffer), "\n================================\n");

    get_buzzer_text(buffer + strlen(buffer), data.hmi.buzzer);
    get_keypad_text(buffer + strlen(buffer), data.hmi.keypad);
    get_system_text(buffer + strlen(buffer), data.hmi.system);
    get_rgb_text(buffer + strlen(buffer), data.hmi.rgb);
    get_display_text(buffer + strlen(buffer), data.hmi.display);
    get_eeprom_text(buffer + strlen(buffer), data.hmi.eeprom);

    sprintf(buffer + strlen(buffer), "\n================================\n");

    bsp_serial_write(buffer);
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
        default:
            break;
    }
}

static void water_level_menu_presenter(void)
{
    bsp_serial_print(WATER_LEVEL_MENU_TARGET_MIN);
    bsp_serial_write(") Update water level target min.\n");
    bsp_serial_print(WATER_LEVEL_MENU_TARGET_MAX);
    bsp_serial_write(") Update water level target max.\n");
    bsp_serial_write("b) Back to calibration.\n");
    bsp_serial_write("> ");
}

static void moisture_menu_presenter(void)
{
    bsp_serial_print(MOIS_MENU_SET_TARGET_MIN);
    bsp_serial_write(") Update soil moisture sensor target min.\n");
    bsp_serial_print(MOIS_MENU_SET_TARGET_MAX);
    bsp_serial_write(") Update soil moisture sensor target max.\n");
    bsp_serial_write("b) Back to calibration.\n");
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

static void light_menu_presenter(void)
{
    bsp_serial_print(LIGHT_MENU_TARGET_MIN);
    bsp_serial_write(") Update light intensity target min.\n");
    bsp_serial_write("b) Back to calibration.\n");
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

static void humidity_menu_presenter(void)
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
    bsp_serial_write("> ");
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

static void temperature_menu_presenter(void)
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
    bsp_serial_write("> ");
}

static void logged_files_menu_handler(void * args)
{

    filelist_t list = sdcard_get_files_list();
    uint8_t i = 0;
    char buffer[255] = {};

    while(list.logs[i] != NULL && i < DAYS)
    {
        if(memcmp(list.logs[i], args, sizeof(char) * FILE_LENGTH) == 0)
        {
            sdcard_read_file(list.logs[i], buffer, sizeof(uint16_t));
            //while()
            bsp_serial_write(buffer);
            bsp_serial_write("\n");
            return;
        }
    }
    bsp_serial_write("Could not find file");
}

static void logged_files_menu_presenter(void)
{
    filelist_t list = sdcard_get_files_list();
    for(uint8_t i = 0; i < 2; i++)
    {
        bsp_serial_write(list.logs[i]);
        bsp_serial_write("\n");
    }
    bsp_serial_write("b) Return to main menu.\n");
    bsp_serial_write("> ");
}

static void main_menu_presenter(void)
{
    bsp_serial_write("---- THE GREENHOUSE ----\n");
    bsp_serial_print(MAIN_MENU_SNAPSHOT);
    bsp_serial_write(") Get snapshop of the system.\n");
    bsp_serial_print(MAIN_MENU_FILES);
    bsp_serial_write(") Get list of logged files.\n");
    bsp_serial_print(MAIN_MENU_CALIBRATION);
    bsp_serial_write(") Calibrate the system.\n");
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

static void breadcrumbs_enter(void (* h)(void *), void (* p)(void))
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
        breadcrumbs.presenters[breadcrumbs.level]();
        allow_display = false;
        allow_read = true;
    }
}

static inline bool in_allowed_chars(char value)
{
    uint8_t index = 0;
    char c;
    do {
	c = *(allowed_chars + index);
	if(c == value)
	{
	    return true;
	}
	else
	{
	    index += 1;
	}
    } while(c != '\0');
   
    return false;
}

static void read()
{
    if(allow_read == true)
    {
	char value = 0;
	if(bsp_serial_available() > 0)
	{
	    value = bsp_serial_read();

	    if(in_allowed_chars(value))
	    {
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
	    else 
	    {
		return;
	    }
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
