/**
 * @author      : jesper (jesper@jesper-HP-ENVY-Laptop-13-aq0xxx)
 * @file        : terminal_new
 * @created     : måndag dec 07, 2020 10:56:45 CET
 */

#include <terminal.h>

#define TERMINAL_STATE_READ 0
#define TERMINAL_STATE_CHECK 1
#define TERMINAL_STATE_EXECUTE 2
#define TERMINAL_STATE_DISPLAY 3

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

static void get_snapshot();

static inline terminal_menu_t *terminal_current_menu();

static void breadcrumbs_enter(terminal_menu_t *menu);
static void breadcrumbs_leave();

static void menu_main_display();
static void menu_main_execute();

static void menu_files_display();
static void menu_files_execute();

void terminal_init(void)
{
    // Define the menus.
    menu_main = terminal_menu_t{"01\n", menu_main_display, menu_main_execute};
    menu_files = terminal_menu_t{"0123456789b\n", menu_files_display, menu_files_execute};

    // Set menu main to be the first menu in breadcrumbs list.
    breadcrumbs_level = 0;
    breadcrumbs[0] = menu_main;

    // The Terminal should start by showing the main menu.
    terminal_state = TERMINAL_STATE_DISPLAY;

    set_terminal_status(CONNECTED);

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

		if(c == '\n')
		{
		    terminal_state = TERMINAL_STATE_EXECUTE;
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
	default:
	    bsp_serial_write("Command not found: ");
	    bsp_serial_write(terminal_input);
	    bsp_serial_write("\n");
	    break;
	    // TODO: add default action;
    }
}

/*

bool calibration_system_display()
{
    char buffer[128] = {};
    sprintf(buffer + strlen(buffer), "\n0) temperature\n");
    sprintf(buffer + strlen(buffer), "> ");

    bsp_serial_write(buffer);
    return true;
}

bool calibrate_temperature_instructions()
{
    char buffer[258] = {};
    sprintf(buffer + strlen(buffer), "\nTargets: \n");
    sprintf(buffer + strlen(buffer), "	0)	target_min\n");
    sprintf(buffer + strlen(buffer), "	1)	target max\n");
    sprintf(buffer + strlen(buffer), "\nCommands: \n");
    sprintf(buffer + strlen(buffer), "	set [target] [value]	Update canbus target with value\n");
    sprintf(buffer + strlen(buffer), "	b)			Return to calibration\n");
    sprintf(buffer + strlen(buffer), "> ");

    bsp_serial_write(buffer);
    return true;
}

bool calibrate_system()
{
    breadcrumbs_enter(cali_term);
    return true;
}

bool calibrate_temperature()
{
    breadcrumbs_enter(temperature_menu);
    return true;
}

*/

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

/*
bool set_canbus_calibration_value()
{
    char *arg;
    char target;
    uint16_t value;

    SerialTerminal *temp = &breadcrumbs[breadcrumbs_level];

    arg = temp->getNext();
    if (arg == NULL)
    {
    }
    target = arg[0];

    arg = temp->getNext();
    if (arg == NULL)
    {
    }
    value = strtol(arg, NULL, 10);

    if (memcmp(temp, &temperature_menu, sizeof(&temperature_menu)) == 0)
    {
        switch (target)
        {
        case '0':
            set_temperature_target_min((uint8_t)value);
            break;
        case '1':
            set_temperature_target_max((uint8_t)value);
            break;
        }
    }

    show_instructions = true;
    return true;
}
*/
