/**
 * @author      : jesper (jesper@jesper-HP-ENVY-Laptop-13-aq0xxx)
 * @file        : terminal_new
 * @created     : måndag dec 07, 2020 10:56:45 CET
 */

#include <bsp.h>
#include <string.h>
#include <candata.h>
#include <canbus.h>
#include <sdcard.h>
#include <stdio.h>
#include <common.h>
#include <stdlib.h>

#include <terminal.h>

#define TERMINAL_STATE_READ 0
#define TERMINAL_STATE_CHECK 1
#define TERMINAL_STATE_EXECUTE 2
#define TERMINAL_STATE_DISPLAY 3

#define MENU_TEMPERATURE '0'
#define SET_TEMPERATURE_CALIBRATION_MIN '0'
#define SET_TEMPERATURE_CALIBRATION_MAX '1'
#define SET_TEMPERATURE_TARGET_MIN '2'
#define SET_TEMPERATURE_TARGET_MAX '3'
#define MENU_LIGHT '1'
#define SET_LIGHT_INTENSITY_TARGET_MIN '0'
#define MENU_HUMIDITY '2'
#define SET_HUMIDITY_CALIBRATION_MIN '0'
#define SET_HUMIDITY_CALIBRATION_MAX '1'
#define SET_HUMIDITY_TARGET_MIN '2'
#define SET_HUMIDITY_TARGET_MAX '3'
#define MENU_MOISTURE '3'
#define MENU_FLOW_METER '4'
#define MENU_WATER_LEVEL '5'
#define SET_WATER_LEVEL_TARGET_MIN '0'
#define SET_WATER_LEVEL_TARGET_MAX '1'

#define TERMINAL_INPUT_MAX 16
#define BREADCRUMB_LEVEL_MAX 6

typedef struct
{
    const char *match;
    void (*display)(void);
    void (*execute)(void);
} terminal_menu_t;

static uint8_t terminal_state = 0;
static char terminal_input[TERMINAL_INPUT_MAX] = {};
static terminal_menu_t breadcrumbs[BREADCRUMB_LEVEL_MAX] = {};
static uint8_t breadcrumbs_level = 0;
static bool execution_completed = true;

static terminal_menu_t menu_main;
static terminal_menu_t menu_files;
static terminal_menu_t menu_calibration;
static terminal_menu_t menu_temperature;
static terminal_menu_t menu_light;
static terminal_menu_t menu_humidity;
static terminal_menu_t menu_moisture;
static terminal_menu_t menu_flow_meter;
static terminal_menu_t menu_water_level;

static void get_snapshot();

static inline terminal_menu_t *terminal_current_menu();

static void breadcrumbs_enter(terminal_menu_t *menu);
static void breadcrumbs_leave();

static void menu_main_display();
static void menu_main_execute();

static void menu_files_display();
static void menu_files_execute();

static void menu_temperature_execute(void);
static void menu_temperature_display(void);
static void menu_light_execute(void);
static void menu_light_display(void);
static void menu_humidity_execute(void);
static void menu_humidity_display(void);
static void menu_moisture_execute(void);
static void menu_moisture_display(void);
static void menu_flow_meter_execute(void);
static void menu_flow_meter_display(void);
static void menu_water_level_execute(void);
static void menu_water_level_display(void);

static void menu_calibration_execute(void);
static void menu_calibration_display(void);

static uint8_t get_next_start_index = 0;
static uint8_t get_next_end_index = 0;

void terminal_init(void)
{
    // Define the menus.
    menu_main = terminal_menu_t{"012\n", menu_main_display, menu_main_execute};
    menu_files = terminal_menu_t{"0123456789b\n", menu_files_display, menu_files_execute};
    menu_calibration = terminal_menu_t{"b01234\n", menu_calibration_display, menu_calibration_execute};
    menu_temperature = terminal_menu_t{"b 0123456789\n", menu_temperature_display, menu_temperature_execute};
    menu_light = terminal_menu_t{"b 023456789\n", menu_light_display, menu_light_execute};
    menu_humidity = terminal_menu_t{"b 0123456789\n", menu_humidity_display, menu_humidity_execute};
    menu_moisture = terminal_menu_t{"b 0123456789\n", menu_moisture_display, menu_moisture_execute};
    menu_flow_meter = terminal_menu_t{"b 0123456789\n", menu_flow_meter_display, menu_flow_meter_execute};
    menu_water_level = terminal_menu_t{"b 0123456789\n", menu_water_level_display, menu_water_level_execute};

    // Set menu main to be the first menu in breadcrumbs list.
    breadcrumbs_level = 0;
    breadcrumbs[0] = menu_main;

    // The Terminal should start by showing the main menu.
    terminal_state = TERMINAL_STATE_DISPLAY;

    set_terminal_status(CONNECTED);
    execution_completed = true;

    bsp_serial_begin();
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

void terminal_run(void)
{
    static char input[2] = {};
    static char c;
    static char check_index;
    static char store_index;

    if(get_sdcard_status() == UNINITIALIZED)
    {
	sdcard_init();
	set_sdcard_status(OKAY);
    }

    switch(terminal_state)
    {
	case TERMINAL_STATE_CHECK:

	    // This means that there are no more characters to match.
	    // So if this is true, return to read state.
	    if(*(terminal_current_menu()->match + check_index) == '\0')
	    {
		terminal_state = TERMINAL_STATE_READ;
	    }

	    // If there are any matching characters, then store the character.
	    if(*(terminal_current_menu()->match + check_index) == c)
	    {
		sprintf(input, "%c", c);
		bsp_serial_write(input);

		// If the character is enter, then continue to execute state.
		// else store the character in the terminal_input buffer.
		if(c == '\n')
		{
		    terminal_state = TERMINAL_STATE_EXECUTE;
		    *(terminal_input + store_index) = '\0';
		    store_index = 0;
		}
		else
		{
		    *(terminal_input + store_index) = c;
		    store_index += 1;
		}
	    }

	    check_index += 1;
	    break;

	case TERMINAL_STATE_READ:
	    if(bsp_serial_available > 0)
	    {
		if((c = bsp_serial_read()) > 0)
		{
		    terminal_state = TERMINAL_STATE_CHECK;
		    check_index = 0;
		}
	    }
	    break;

	case TERMINAL_STATE_EXECUTE:
	    get_next_start_index = 0;
	    get_next_end_index = 0;
	    terminal_current_menu()->execute();
	    if(execution_completed)
	    {
		terminal_state = TERMINAL_STATE_DISPLAY;

		//Gotta reset that input after we executed out command.
		memset(input, 0, sizeof(input));
	    }
	    break;

	case TERMINAL_STATE_DISPLAY:
	    terminal_current_menu()->display();
	    if(execution_completed)
	    {
		terminal_state = TERMINAL_STATE_READ;
	    }
	    break;
    }
}

static void get_next(char * buffer)
{
  do
  {
    Serial.printf("Output: %d - %c\n", get_next_end_index, terminal_input[get_next_end_index]);
    get_next_end_index++;
  } while(terminal_input[get_next_end_index] != '\0' || terminal_input[get_next_end_index] != ' ');
  memcpy(buffer, terminal_input + get_next_start_index, get_next_end_index);
  get_next_start_index = get_next_end_index;
}

static inline terminal_menu_t * terminal_current_menu(void)
{
    return &breadcrumbs[breadcrumbs_level];
}

static void breadcrumbs_enter(terminal_menu_t * menu)
{
    breadcrumbs_level += 1U;
    breadcrumbs[breadcrumbs_level] = *menu;
}

static void breadcrumbs_leave()
{
    breadcrumbs_level -= 1U;
}

static void menu_main_display(void)
{
    char buffer[128] = {};
    sprintf(buffer + strlen(buffer), "\n0) Get Snapshot");
    sprintf(buffer + strlen(buffer), "\n1) List logged files");
    sprintf(buffer + strlen(buffer), "\n2) Calibrate system");
    sprintf(buffer + strlen(buffer), "\n> ");

    bsp_serial_write(buffer);
}

static void menu_main_execute(void)
{
    switch(terminal_input[0])
    {
	case '0':
	    get_snapshot();
	    break;
	case '1':
	    breadcrumbs_enter(&menu_files);
	    break;
	case '2':
	    breadcrumbs_enter(&menu_calibration);
	    break;
	default:
	    bsp_serial_write("Command not found: ");
	    bsp_serial_write(terminal_input);
	    bsp_serial_write("\n");
	    break;
	    // TODO: add default action;
    }
}

static void menu_files_display(void)
{
    char buffer[258] = {};
    filelist_t list = sdcard_get_files_list();
    static uint16_t day = 0;

    if(list.logs[day][0] == '\0')
    {
	day = 0;
	sprintf(buffer + strlen(buffer), "b)			Return to main menu\n");
	sprintf(buffer + strlen(buffer), "\nEnter file name: \n");
	sprintf(buffer + strlen(buffer), "> ");
	bsp_serial_write(buffer);
	execution_completed = true;
    }
    else
    {
	bsp_serial_write(list.logs[day]);
	bsp_serial_write("\n");
	day++;
	execution_completed = false;
    }
}

static void menu_files_execute(void)
{
    char buffer[16 * 1024] = {};
    if(terminal_input[0] == 'b')
    {
	breadcrumbs_leave();
    }

    if (SUCCESS == sdcard_read_file(terminal_input, buffer, sizeof(buffer) - 1))
    {
        if(*buffer)
        {
            bsp_serial_write(buffer);
	    execution_completed = false;
        }
        else
        {
	    breadcrumbs_enter(&menu_files);
	    execution_completed = true;
        }
    }
}

void menu_calibration_execute(void)
{
  switch(terminal_input[0])
  {
    case MENU_TEMPERATURE:
      breadcrumbs_enter(&menu_temperature);
      break;
    case MENU_LIGHT:
      breadcrumbs_enter(&menu_light);
      break;
    case MENU_HUMIDITY:
      breadcrumbs_enter(&menu_humidity);
      break;
    case MENU_MOISTURE:
      breadcrumbs_enter(&menu_moisture);
      break;
    case MENU_FLOW_METER:
      breadcrumbs_enter(&menu_flow_meter);
      break;
    case MENU_WATER_LEVEL:
      breadcrumbs_enter(&menu_water_level);
      break;

  }
}


//temperature execute

static void menu_temperature_execute()
{
  char command_ptr[2];
  char value_ptr[5];
  get_next(command_ptr);
  get_next(value_ptr);

  char output[128];

  if(!command_ptr || !value_ptr)
  {
    return;
  }
  char command = command_ptr[0];
  bsp_serial_write(terminal_input);
  uint8_t value = strtol(value_ptr, NULL, 10);

  switch(command) 
  {
    case SET_TEMPERATURE_CALIBRATION_MIN:
      set_temperature_calibration_min(value);
      sprintf(output, "%s updated to: %d."," temperature calibration min",value);
      bsp_serial_write(output);
      break;
    case SET_TEMPERATURE_CALIBRATION_MAX:
      set_temperature_calibration_max(value);
      sprintf(output, "%s updated to: %d."," temperature calibration max",value);
      bsp_serial_write(output);
      break;
    case SET_TEMPERATURE_TARGET_MIN:
      set_temperature_target_min(value);
      sprintf(output, "%s updated to: %d."," temperature target min",value);
      bsp_serial_write(output);
      break;
    case SET_TEMPERATURE_TARGET_MAX:
      set_temperature_target_max(value);
      sprintf(output, "%s updated to: %d."," temperature target max",value);
      bsp_serial_write(output);
      break;
    case 'b':
      breadcrumbs_leave();
      break;
    default:
      break;
  }
}

//light execute

static void menu_light_execute()
{
  char command_ptr[2];
  char value_ptr[5];
  get_next(command_ptr);
  get_next(value_ptr);

  char output[128];

  if(!command_ptr || !value_ptr)
  {
    return;
  }
  char command = command_ptr[0];
  uint8_t value = strtol(value_ptr, NULL, 10);

  switch(command) 
  {
    case SET_LIGHT_INTENSITY_TARGET_MIN:
      set_light_intensity_target_min(value);
      sprintf(output, "%s updated to: %d."," light intensity target min",value);
      bsp_serial_write(output);
      break;
    case 'b':
      breadcrumbs_leave();
      break;
    default:
      break;
  }
}

//humidity execute

static void menu_humidity_execute()
{
  char command_ptr[2];
  char value_ptr[5];
  get_next(command_ptr);
  get_next(value_ptr);

  char output[128];

  if(!command_ptr || !value_ptr)
  {
    return;
  }
  char command = command_ptr[0];
  uint8_t value = strtol(value_ptr, NULL, 10);

  switch(command) 
  {
    case SET_HUMIDITY_CALIBRATION_MIN:
      set_humidity_calibration_min(value);
      sprintf(output, "%s updated to: %d."," humidity calibration min",value);
      bsp_serial_write(output);
      break;
    case SET_HUMIDITY_CALIBRATION_MAX:
      set_humidity_calibration_max(value);
      sprintf(output, "%s updated to: %d."," humidity calibration max",value);
      bsp_serial_write(output);
      break;
    case SET_HUMIDITY_TARGET_MIN:
      set_humidity_target_min(value);
      sprintf(output, "%s updated to: %d."," humidity target min",value);
      bsp_serial_write(output);
      break;
    case SET_HUMIDITY_TARGET_MAX:
      set_humidity_target_max(value);
      sprintf(output, "%s updated to: %d."," humidity target max",value);
      bsp_serial_write(output);
      break;
    case 'b':
      breadcrumbs_leave();
      break;
    default:
      break;
  }
}

//moisture execute

static void menu_moisture_execute()
{
  char command_ptr[2];
  char value_ptr[5];
  get_next(command_ptr);
  get_next(value_ptr);

  //char output[128];

  if(!command_ptr || !value_ptr)
  {
    return;
  }
  char command = command_ptr[0];
  //uint8_t value = strtol(value_ptr, NULL, 10);

  switch(command) 
  {
    case 'b':
      breadcrumbs_leave();
      break;
    default:
      break;
  }
}

//flow_meter execute

static void menu_flow_meter_execute()
{
  char command_ptr[2];
  char value_ptr[5];
  get_next(command_ptr);
  get_next(value_ptr);

  //char output[128];

  if(!command_ptr || !value_ptr)
  {
    return;
  }
  char command = command_ptr[0];
  //uint8_t value = strtol(value_ptr, NULL, 10);

  switch(command) 
  {
    case 'b':
      breadcrumbs_leave();
      break;
    default:
      break;
  }
}

//water_level execute

static void menu_water_level_execute()
{
  char command_ptr[2];
  char value_ptr[5];
  get_next(command_ptr);
  get_next(value_ptr);

  char output[128];

  if(!command_ptr || !value_ptr)
  {
    return;
  }
  char command = command_ptr[0];
  uint8_t value = strtol(value_ptr, NULL, 10);

  switch(command) 
  {
    case SET_WATER_LEVEL_TARGET_MIN:
      set_water_level_target_min(value);
      sprintf(output, "%s updated to: %d."," water level target min",value);
      bsp_serial_write(output);
      break;
    case SET_WATER_LEVEL_TARGET_MAX:
      set_water_level_target_max(value);
      sprintf(output, "%s updated to: %d."," water level target max",value);
      bsp_serial_write(output);
      break;
    case 'b':
      breadcrumbs_leave();
      break;
    default:
      break;
  }
}

// Calibration display

static void menu_calibration_display(void)
{
  char buffer[256] = {};
  sprintf(buffer + strlen(buffer), "%c) Temperature\n", MENU_TEMPERATURE);
  sprintf(buffer + strlen(buffer), "%c) Light\n", MENU_LIGHT);
  sprintf(buffer + strlen(buffer), "%c) Humidity\n", MENU_HUMIDITY);
  sprintf(buffer + strlen(buffer), "%c) Moisture\n", MENU_MOISTURE);
  sprintf(buffer + strlen(buffer), "%c) Flow meter\n", MENU_FLOW_METER);
  sprintf(buffer + strlen(buffer), "%c) Water level\n", MENU_WATER_LEVEL);
  sprintf(buffer + strlen(buffer), "b) return\n");
  sprintf(buffer + strlen(buffer), ">");
  bsp_serial_write(buffer);
}

//temperature display

static void menu_temperature_display()
{
  char buffer[256] = {};
  sprintf(buffer + strlen(buffer), "%c) set temperature calibration min\n", SET_TEMPERATURE_CALIBRATION_MIN);
  sprintf(buffer + strlen(buffer), "%c) set temperature calibration max\n", SET_TEMPERATURE_CALIBRATION_MAX);
  sprintf(buffer + strlen(buffer), "%c) set temperature target min\n", SET_TEMPERATURE_TARGET_MIN);
  sprintf(buffer + strlen(buffer), "%c) set temperature target max\n", SET_TEMPERATURE_TARGET_MAX);
  sprintf(buffer + strlen(buffer), "b) return\n");
  sprintf(buffer + strlen(buffer), "Enter [target] [value] >");
  bsp_serial_write(buffer);
}


//light display

static void menu_light_display()
{
  char buffer[256] = {};
  sprintf(buffer + strlen(buffer), "%c) set light intensity target min\n", SET_LIGHT_INTENSITY_TARGET_MIN);
  sprintf(buffer + strlen(buffer), "b) return\n");
  sprintf(buffer + strlen(buffer), "Enter [target] [value] >");
  bsp_serial_write(buffer);
}


//humidity display

static void menu_humidity_display()
{
  char buffer[256] = {};
  sprintf(buffer + strlen(buffer), "%c) set humidity calibration min\n", SET_HUMIDITY_CALIBRATION_MIN);
  sprintf(buffer + strlen(buffer), "%c) set humidity calibration max\n", SET_HUMIDITY_CALIBRATION_MAX);
  sprintf(buffer + strlen(buffer), "%c) set humidity target min\n", SET_HUMIDITY_TARGET_MIN);
  sprintf(buffer + strlen(buffer), "%c) set humidity target max\n", SET_HUMIDITY_TARGET_MAX);
  sprintf(buffer + strlen(buffer), "b) return\n");
  sprintf(buffer + strlen(buffer), "Enter [target] [value] >");
  bsp_serial_write(buffer);
}


//moisture display

static void menu_moisture_display()
{
  char buffer[256] = {};
  sprintf(buffer + strlen(buffer), "b) return\n");
  sprintf(buffer + strlen(buffer), "Enter [target] [value] >");
  bsp_serial_write(buffer);
}


//flow_meter display

static void menu_flow_meter_display()
{
  char buffer[256] = {};
  sprintf(buffer + strlen(buffer), "b) return\n");
  sprintf(buffer + strlen(buffer), "Enter [target] [value] >");
  bsp_serial_write(buffer);
}


//water_level display

static void menu_water_level_display()
{
  char buffer[256] = {};
  sprintf(buffer + strlen(buffer), "%c) set water level target min\n", SET_WATER_LEVEL_TARGET_MIN);
  sprintf(buffer + strlen(buffer), "%c) set water level target max\n", SET_WATER_LEVEL_TARGET_MAX);
  sprintf(buffer + strlen(buffer), "b) return\n");
  sprintf(buffer + strlen(buffer), "Enter [target] [value] >");
  bsp_serial_write(buffer);
}
