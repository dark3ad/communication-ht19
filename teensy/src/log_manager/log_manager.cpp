/**
 * @file log_manager.cpp
 * @author Patrik Gustafsson (patrik-emil.gustafsson@yaelev.se)
 * @brief Log manager handles logging changes on canbus. Includes functionality for date-dependent log rotation, log analysis, error handling
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <common.h>
#include <canbus.h>
#include <sdcard.h>
#include <string.h>
#include <candata.h>
#include <bsp_time.h>
#include <led_driver.h>
#include <log_manager.h>

static void write_error(const char *err_msg);
static uint16_t refresh_data(data_t current, char *buffer);

static uint8_t today = 0;
static data_t previous = {};

void log_manager_init(void)
{
    led_driver_init();

    uint8_t status = sdcard_init();

    if (SUCCESS == status)
    {
        uint8_t sdcard_status = sdcard_create_file(ERROR_LOG);
        if ((sdcard_status != SUCCESS) && (sdcard_status != FILE_EXIST))
        {
            status = CREATE_FILE_ERROR;
        }
    }

    set_sdcard_status((status == SUCCESS) ? OKAY : ERROR);
}

void log_manager_run(void)
{
    if ((UNINITIALIZED != get_rtc_status()) && (UNINITIALIZED != get_sdcard_status()))
    {
        uint8_t temp = bsp_day();
        uint8_t sdcard_status = OKAY;
        data_t current = get_candata();
        char today_file[FILE_LENGTH] = {};
        char temp_err[256] = {};

        sprintf(today_file, "%02d", today);

        if (temp != today)
        {
            today = temp;
            sprintf(today_file, "%02d", today);

            if (REMOVE_FILE_ERROR == sdcard_delete_file(today_file))
            {
                memset(temp_err, 0, sizeof(temp_err));
                sprintf(temp_err, "[%s] Failed to delete %s\n", current.communication.rtc.datetime, today_file);
                write_error(temp_err);
                sdcard_status = ERROR;
            }
            else if (SUCCESS != sdcard_create_file(today_file))
            {
                memset(temp_err, 0, sizeof(temp_err));
                sprintf(temp_err, "[%s] Failed to create %s\n", current.communication.rtc.datetime, today_file);
                write_error(temp_err);
                sdcard_status = ERROR;
            }
        }

        if (OKAY == sdcard_status)
        {
            char text[1024] = {};
            sprintf(text, "[%s] Changed data\n----------------------------------\n", current.communication.rtc.datetime);

            if (refresh_data(current, text) > 0)
            {
                if (SUCCESS != sdcard_append_file(today_file, text))
                {
                    memset(temp_err, 0, sizeof(temp_err));
                    sprintf(temp_err, "[%s] Failed to write to file: %s\n", current.communication.rtc.datetime, today_file);
                    write_error(temp_err);
                    sdcard_status = ERROR;
                }
            }
        }

        uint16_t space = sdcard_get_free_space();
        if (space > 0)
        {
            set_sdcard_free_space(space);
        }
        else
        {
            memset(temp_err, 0, sizeof(temp_err));
            sprintf(temp_err, "[%s] Failed to retreive sdcard free space\n", current.communication.rtc.datetime);
            write_error(temp_err);
            sdcard_status = ERROR;
        }

        set_sdcard_status(sdcard_status);
    }
}

static void write_error(const char *err_msg)
{
    if (SUCCESS == sdcard_append_file(ERROR_LOG, err_msg))
    {
        led_driver_turn_off();
    }
    else
    {
        led_driver_turn_on();
    }
}

static uint16_t refresh_data(data_t current, char *buffer)
{
    uint16_t length = 0;

    if (is_buzzer_changed(previous.hmi.buzzer, current.hmi.buzzer))
    {
        length = strlen(buffer);
        get_buzzer_text(buffer + length, current.hmi.buzzer);
    }

    if (is_flow_meter_changed(previous.sensor.flow_meter, current.sensor.flow_meter))
    {
        length = strlen(buffer);
        get_flow_meter_text(buffer + length, current.sensor.flow_meter);
    }

    if (is_moisture_changed(previous.sensor.moisture, current.sensor.moisture))
    {
        length = strlen(buffer);
        get_moisture_text(buffer + length, current.sensor.moisture);
    }

    if (is_display_changed(previous.hmi.display, current.hmi.display))
    {
        length = strlen(buffer);
        get_display_text(buffer + length, current.hmi.display);
    }
    if (is_eeprom_changed(previous.hmi.eeprom, current.hmi.eeprom))
    {
        length = strlen(buffer);
        get_eeprom_text(buffer + length, current.hmi.eeprom);
    }

    if (is_esp32_changed(previous.communication.esp32, current.communication.esp32))
    {
        length = strlen(buffer);
        get_esp32_text(buffer + length, current.communication.esp32);
    }

    if (is_fans_changed(previous.actuator.fans, current.actuator.fans))
    {
        length = strlen(buffer);
        get_fans_text(buffer + length, current.actuator.fans);
    }

    if (is_heater_changed(previous.actuator.heater, current.actuator.heater))
    {
        length = strlen(buffer);
        get_heater_text(buffer + length, current.actuator.heater);
    }

    if (is_humidity_changed(previous.sensor.humidity, current.sensor.humidity))
    {
        length = strlen(buffer);
        get_humidity_text(buffer + length, current.sensor.humidity);
    }

    if (is_keypad_changed(previous.hmi.keypad, current.hmi.keypad))
    {
        length = strlen(buffer);
        get_keypad_text(buffer + length, current.hmi.keypad);
    }

    if (is_lamp_changed(previous.actuator.lamp, current.actuator.lamp))
    {
        length = strlen(buffer);
        get_lamp_text(buffer + length, current.actuator.lamp);
    }

    if (is_light_changed(previous.sensor.light, current.sensor.light))
    {
        length = strlen(buffer);
        get_light_text(buffer + length, current.sensor.light);
    }

    if (is_rgb_changed(previous.hmi.rgb, current.hmi.rgb))
    {
        length = strlen(buffer);
        get_rgb_text(buffer + length, current.hmi.rgb);
    }

    if (is_rtc_changed(previous.communication.rtc, current.communication.rtc))
    {
        length = strlen(buffer);
        get_rtc_text(buffer + length, current.communication.rtc);
    }

    if (is_sdcard_changed(previous.communication.sdcard, current.communication.sdcard))
    {
        length = strlen(buffer);
        get_sdcard_text(buffer + length, current.communication.sdcard);
    }

    if (is_system_changed(previous.hmi.system, current.hmi.system))
    {
        length = strlen(buffer);
        get_system_text(buffer + length, current.hmi.system);
    }

    if (is_temperature_changed(previous.sensor.temperature, current.sensor.temperature))
    {
        length = strlen(buffer);
        get_temperature_text(buffer + length, current.sensor.temperature);
    }

    if (is_terminal_changed(previous.communication.terminal, current.communication.terminal))
    {
        length = strlen(buffer);
        get_terminal_text(buffer + length, current.communication.terminal);
    }

    if (is_water_level_changed(previous.sensor.water_level, current.sensor.water_level))
    {
        length = strlen(buffer);
        get_water_level_text(buffer + length, current.sensor.water_level);
    }

    if (is_water_pump_changed(previous.actuator.water_pump, current.actuator.water_pump))
    {
        length = strlen(buffer);
        get_water_pump_text(buffer + length, current.actuator.water_pump);
    }

    if (is_water_valve_changed(previous.actuator.water_valve, current.actuator.water_valve))
    {
        length = strlen(buffer);
        get_water_valve_text(buffer + length, current.actuator.water_valve);
    }

    if (is_window_changed(previous.actuator.window, current.actuator.window))
    {
        length = strlen(buffer);
        get_window_text(buffer + length, current.actuator.window);
    }

    if (length > 0)
    {
        strcat(buffer, "\n\n");
        length += 2;
    }

    previous = current;

    return length;
}