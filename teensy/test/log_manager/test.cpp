/**
 * @file test.cpp
 * @author Patrik Gustafsson (patrik-emil.gustafsson@yaelev.se)
 * @brief Test for log_manager app and sdcard driver and other used modules
 * @version 0.1
 * @date 2020-11-23
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifdef TARGET
#include <Arduino.h>
#include <TimeLib.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif

#include <unity.h>
#include <stdio.h>
#include <common.h>
#include <canbus.h>
#include <string.h>
#include <sdcard.h>
#include <candata.h>
#include <bsp_time.h>
#include <led_driver.h>
#include <log_manager.h>

/********* sdcard  ********/
typedef struct
{
    uint8_t init_status;
    uint16_t free_space;
    uint8_t read_status;
    uint8_t delete_status;
    uint8_t create_status;
    uint8_t append_status;
} sdcard_test_t;

/********* bsp_time  ********/
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} date_test_t;

/********* misc ********/
static date_test_t date = {};
static uint8_t led_state = LOW;
static sdcard_test_t sdcard = {};

/********* sdcard test double ********/
static bool check_file_name(const char *name)
{
    bool valid = false;
    if (!strcmp(name, ERROR_LOG))
    {
        valid = true;
    }
    else
    {
        for (uint8_t i = 1; i <= DAYS; i++)
        {
            char temp[FILE_LENGTH] = {};
            sprintf(temp, "%02d", i);
            if (!strcmp(temp, name))
            {
                valid = true;
                break;
            }
        }
    }
    return valid;
}

uint8_t sdcard_init(void)
{

#ifdef TARGET

#endif
    return sdcard.init_status;
}

uint16_t sdcard_get_free_space(void)
{
#ifdef TARGET

#endif

    return sdcard.free_space;
}

uint8_t sdcard_delete_file(const char *file_name)
{
    if (sdcard.delete_status != SUCCESS)
    {
        return sdcard.delete_status;
    }

    sdcard.delete_status = FILE_NOT_EXIST;

#ifdef TARGET

#else
    int fd = open(file_name, O_RDONLY);
    if (fd >= 0)
    {
        close(fd);
        if (remove(file_name) == 0)
        {
            sdcard.delete_status = SUCCESS;
        }
        else
        {
            sdcard.delete_status = REMOVE_FILE_ERROR;
        }
    }
#endif

    return sdcard.delete_status;
}
uint8_t sdcard_create_file(const char *file_name)
{
    if (sdcard.create_status != SUCCESS)
    {
        return sdcard.create_status;
    }

    if (!check_file_name(file_name))
    {
        sdcard.create_status = INVALID_FILE_NAME;
        return sdcard.create_status;
    }

    sdcard.create_status = CREATE_FILE_ERROR;

#ifdef TARGET

#else
    int fd = open(file_name, O_RDONLY);
    if (fd < 0)
    {
        fd = creat(file_name, O_CREAT | O_EXCL | S_IRUSR);
        if (fd >= 0)
        {
            close(fd);
            sdcard.create_status = SUCCESS;
        }
    }
    else
    {
        close(fd);
        sdcard.create_status = FILE_EXIST;
    }
#endif

    return sdcard.create_status;
}

uint8_t sdcard_append_file(const char *file_name, const char *text)
{
    if (sdcard.append_status == WRITE_FILE_ERROR)
    {
        return sdcard.append_status;
    }

    if (!check_file_name(file_name))
    {
        sdcard.append_status = INVALID_FILE_NAME;
        return sdcard.append_status;
    }

#ifdef TARGET

#else
    int fd = open(file_name, O_RDWR | O_APPEND);
    if (fd < 0)
    {
        sdcard.append_status = FILE_NOT_EXIST;
    }
    else
    {
        size_t p = strlen(text);
        if (p != write(fd, text, p))
        {
            sdcard.append_status = WRITE_FILE_ERROR;
        }
        close(fd);
    }
#endif

    return sdcard.append_status;
}

uint8_t sdcard_read_file(const char *file_name, char *buffer, uint16_t length)
{
    if (sdcard.read_status != SUCCESS)
    {
        return sdcard.read_status;
    }

    static uint32_t position = 0;

    if (!check_file_name(file_name))
    {
        position = 0;
        sdcard.read_status = INVALID_FILE_NAME;
        return sdcard.read_status;
    }

#ifdef TARGET

#else
    int fd = open(file_name, O_RDONLY);
    if (fd < 0)
    {
        position = 0;
        sdcard.read_status = FILE_NOT_EXIST;
    }
    else
    {
        memset(buffer, 0, length);
        lseek(fd, position, SEEK_SET);

        length--;
        ssize_t size = read(fd, buffer, length);

        if (size < 0)
        {
            sdcard.read_status = READ_FILE_ERROR;
        }
        else if (size < length)
        {
            position = 0;
        }
        else
        {
            position += size;
        }
        close(fd);
    }
#endif

    return sdcard.read_status;
}

/********* led_driver test double ********/
uint8_t led_driver_get_state(void)
{
#ifdef TARGET
    led_state = digitalRead(LED_BUILTIN);
#endif
    return led_state;
}

uint8_t led_driver_init(void)
{
    led_state = LOW;
#ifdef TARGET
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    led_state = digitalRead(LED_BUILTIN);
    if (LOW != led_state)
    {
        return ERROR;
    }
#endif
    return OKAY;
}

uint8_t led_driver_turn_on(void)
{
    led_state = HIGH;
#ifdef TARGET
    digitalWrite(LED_BUILTIN, HIGH);
    led_state = digitalRead(LED_BUILTIN);
    if (HIGH != led_state)
    {
        return ERROR;
    }
#endif
    return OKAY;
}

uint8_t led_driver_turn_off(void)
{
    led_state = LOW;
#ifdef TARGET
    digitalWrite(LED_BUILTIN, LOW);
    led_state = digitalRead(LED_BUILTIN);
    if (LOW != led_state)
    {
        return ERROR;
    }
#endif
    return OKAY;
}

/********* bsp_time test double ********/

uint16_t bsp_year(void)
{
#ifdef TARGET
    return year();
#else
    return date.year;
#endif
}
uint8_t bsp_month(void)
{
#ifdef TARGET
    return month();
#else
    return date.month;
#endif
}
uint8_t bsp_day(void)
{
#ifdef TARGET
    return day();
#else
    return date.day;
#endif
}
uint8_t bsp_hour(void)
{
#ifdef TARGET
    return hour();
#else
    return date.hour;
#endif
}
uint8_t bsp_minute(void)
{
#ifdef TARGET
    return minute();
#else
    return date.minute;
#endif
}
uint8_t bsp_second(void)
{
#ifdef TARGET
    return second();
#else
    return date.second;
#endif
}
void bsp_set_time(uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, uint8_t _second)
{
#ifdef TARGET
    setTime(_hour, _minute, _second, _day, _month, _year);
#else
    date.year = _year;
    date.month = _month;
    date.day = _day;
    date.hour = _hour;
    date.minute = _minute;
    date.second = _second;
#endif
    set_rtc_year(_year);
    set_rtc_month(_month);
    set_rtc_day(_day);
    set_rtc_hour(_hour);
    set_rtc_minute(_minute);
    set_rtc_second(_second);
}

void setUp(void)
{
}

void tearDown(void)
{
}

static void reset_sdcard(void)
{
    /* Test reset values for sdcard mimic operations */

    sdcard.init_status = SUCCESS;
    sdcard.read_status = SUCCESS;
    sdcard.create_status = SUCCESS;
    sdcard.append_status = SUCCESS;
    sdcard.delete_status = SUCCESS;
}

void test_log_manager_init(void)
{
    /* Test for log_manager_init error */

    reset_sdcard();
    sdcard.init_status = SDCARD_BEGIN_ERROR;
    log_manager_init();
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_sdcard_status());

    /* Test for log_manager_init SUCCESS but create_file ERROR */

    reset_sdcard();
    set_sdcard_status(OKAY);
    sdcard.create_status = CREATE_FILE_ERROR;
    log_manager_init();
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_sdcard_status());

    /* Test for log_manager_init SUCCESS and create_file SUCCESS */

    reset_sdcard();
    log_manager_init();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_sdcard_status());
    TEST_ASSERT_EQUAL_UINT8(FILE_EXIST, sdcard_create_file(ERROR_LOG));
}

void test_candata(void)
{
    /* Test for candata that is unreachable due to mimic */

    TEST_ASSERT_TRUE(set_esp32_status(NTP_ERROR));
    TEST_ASSERT_EQUAL_UINT8(NTP_ERROR, get_esp32_status());

    TEST_ASSERT_TRUE(set_esp32_status(WIFI_DISCONNECTED));
    TEST_ASSERT_EQUAL_UINT8(WIFI_DISCONNECTED, get_esp32_status());

    TEST_ASSERT_TRUE(set_esp32_status(MQTT_DISCONNECTED));
    TEST_ASSERT_EQUAL_UINT8(MQTT_DISCONNECTED, get_esp32_status());

    TEST_ASSERT_TRUE(set_esp32_status(MQTT_DISCONNECTED));
    TEST_ASSERT_EQUAL_UINT8(MQTT_DISCONNECTED, get_esp32_status());

    TEST_ASSERT_TRUE(set_test_dht_sensor_status(OKAY));
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_dht_sensor_status());
}

void test_log_manager_run(void)
{
    /* Test for different cases of log_manager_run function */

    reset_sdcard();
    set_sdcard_status(OKAY);
    set_rtc_status(UNINITIALIZED);
    log_manager_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_sdcard_status());
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_rtc_status());

    reset_sdcard();
    set_rtc_status(OKAY);
    bsp_set_time(2020, 4, 15, 20, 45, 5);
    sdcard.free_space = 0;
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_rtc_status());
    log_manager_run();
    TEST_ASSERT_EQUAL_UINT8(2020, bsp_year());
    TEST_ASSERT_EQUAL_UINT8(4, bsp_month());
    TEST_ASSERT_EQUAL_UINT8(15, bsp_day());
    TEST_ASSERT_EQUAL_UINT8(20, bsp_hour());
    TEST_ASSERT_EQUAL_UINT8(45, bsp_minute());
    TEST_ASSERT_EQUAL_UINT8(5, bsp_second());
    TEST_ASSERT_EQUAL_UINT16(0, get_sdcard_free_space());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_sdcard_status());

    reset_sdcard();
    sdcard.free_space = 1000;
    bsp_set_time(2020, 4, 16, 20, 45, 5);
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_rtc_status());
    log_manager_run();
    TEST_ASSERT_EQUAL_UINT16(1000, get_sdcard_free_space());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_sdcard_status());

    reset_sdcard();
    sdcard.free_space = 1200;
    bsp_set_time(2020, 4, 17, 20, 45, 6);
    set_test_flow_meter_sensor_status(ERROR);
    set_test_buzzer_status(ERROR);
    set_test_temperature(15);
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_rtc_status());
    log_manager_run();
    TEST_ASSERT_EQUAL_UINT8(15, get_temperature());
    TEST_ASSERT_EQUAL_UINT16(1200, get_sdcard_free_space());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_sdcard_status());

    reset_sdcard();
    sdcard.free_space = 1000;
    bsp_set_time(2020, 4, 18, 20, 45, 5);
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_rtc_status());
    log_manager_run();
    TEST_ASSERT_EQUAL_UINT16(1000, get_sdcard_free_space());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_sdcard_status());

    reset_sdcard();
    sdcard.delete_status = REMOVE_FILE_ERROR;
    bsp_set_time(2020, 4, 19, 20, 45, 5);
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_rtc_status());
    log_manager_run();
    TEST_ASSERT_EQUAL_UINT8(REMOVE_FILE_ERROR, sdcard.delete_status);
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_sdcard_status());

    reset_sdcard();
    sdcard.create_status = CREATE_FILE_ERROR;
    bsp_set_time(2020, 4, 20, 20, 45, 5);
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_rtc_status());
    log_manager_run();
    TEST_ASSERT_EQUAL_UINT8(LOW, led_driver_get_state());
    TEST_ASSERT_EQUAL_UINT8(CREATE_FILE_ERROR, sdcard.create_status);
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_sdcard_status());

    reset_sdcard();
    set_test_soil_moisture_sensor_status(OKAY);
    bsp_set_time(2020, 4, 20, 20, 45, 5);                             // DOESNT WRITE TO ERRORS ( APPEND )
    sdcard.append_status = WRITE_FILE_ERROR;                          // DOESNT WRITE TO ERRORS ( APPEND )
    log_manager_run();                                                // DOESNT WRITE TO ERRORS ( APPEND )
    TEST_ASSERT_EQUAL_UINT8(HIGH, led_driver_get_state());            // DOESNT WRITE TO ERRORS ( APPEND )
    TEST_ASSERT_EQUAL_UINT8(WRITE_FILE_ERROR, sdcard.append_status);  // DOESNT WRITE TO ERRORS ( APPEND )
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_soil_moisture_sensor_status()); // DOESNT WRITE TO ERRORS ( APPEND )
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_sdcard_status());              // DOESNT WRITE TO ERRORS ( APPEND )

    reset_sdcard();
    set_test_fans_status(ERROR);
    set_test_display_status(ERROR);
    set_test_eeprom_status(ERROR);
    set_esp32_status(ERROR);
    set_test_heater_status(ERROR);
    set_test_humidity(ERROR);
    set_test_keypad_status(ERROR);
    set_test_lamp_status(ERROR);
    set_test_light_intensity_sensor_status(ERROR);
    set_test_rgb_status(ERROR);
    set_test_system_status(ERROR);
    set_terminal_status(ERROR);
    set_test_water_level_sensor_status(ERROR);
    set_test_water_pump_status(ERROR);
    set_test_water_valve_status(ERROR);
    set_test_window_status(ERROR);
    log_manager_run();
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_fans_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_display_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_eeprom_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_esp32_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_heater_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_humidity());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_keypad_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_lamp_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_light_intensity_sensor_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_rgb_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_system_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_terminal_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_water_level_sensor_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_water_pump_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_water_valve_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_window_status());
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_sdcard_status());

    TEST_ASSERT_TRUE(set_test_flow_rate(200));
    TEST_ASSERT_EQUAL_UINT16(200, get_flow_rate());

    TEST_ASSERT_TRUE(set_test_light_intensity(80));
    TEST_ASSERT_EQUAL_UINT8(80, get_light_intensity());

    TEST_ASSERT_TRUE(set_test_water_level(60));
    TEST_ASSERT_EQUAL_UINT8(60, get_water_level());

    TEST_ASSERT_TRUE(set_test_soil_moisture(60));
    TEST_ASSERT_EQUAL_UINT8(60, get_soil_moisture());

    TEST_ASSERT_TRUE(set_test_dht_sensor_status(OKAY));
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_dht_sensor_status());

    TEST_ASSERT_TRUE(set_test_lamp_shininess(80));
    TEST_ASSERT_EQUAL_UINT8(80, get_lamp_shininess());

    TEST_ASSERT_TRUE(set_test_heater_warmness(60));
    TEST_ASSERT_EQUAL_UINT8(60, get_heater_warmness());

    TEST_ASSERT_TRUE(set_test_water_pump_state(ON));
    TEST_ASSERT_EQUAL_UINT8(ON, get_water_pump_state());

    TEST_ASSERT_TRUE(set_test_water_valve_state(ON));
    TEST_ASSERT_EQUAL_UINT8(ON, get_water_valve_state());

    TEST_ASSERT_TRUE(set_test_window_state(OPEN));
    TEST_ASSERT_EQUAL_UINT8(OPEN, get_window_state());

    TEST_ASSERT_TRUE(set_test_fans_state(ON));
    TEST_ASSERT_EQUAL_UINT8(ON, get_fans_state());

    TEST_ASSERT_TRUE(set_test_temperature_calibration_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_temperature_calibration_min());

    TEST_ASSERT_TRUE(set_test_temperature_calibration_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_temperature_calibration_max());

    TEST_ASSERT_TRUE(set_test_temperature_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_temperature_target_min());

    TEST_ASSERT_TRUE(set_test_temperature_target_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_temperature_target_max());

    TEST_ASSERT_TRUE(set_test_humidity_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_humidity_target_min());

    TEST_ASSERT_TRUE(set_test_humidity_target_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_humidity_target_max());

    TEST_ASSERT_TRUE(set_test_light_intensity_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_light_intensity_target_min());

    TEST_ASSERT_TRUE(set_test_water_level_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_water_level_target_min());

    TEST_ASSERT_TRUE(set_test_water_level_target_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_water_level_target_max());

    TEST_ASSERT_TRUE(set_test_soil_moisture_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_soil_moisture_target_min());

    TEST_ASSERT_TRUE(set_test_soil_moisture_target_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_soil_moisture_target_max());

    TEST_ASSERT_TRUE(set_test_water_volume_calibration_max(1000));
    TEST_ASSERT_EQUAL_UINT16(1000, get_water_volume_calibration_max());

    TEST_ASSERT_TRUE(set_temperature_calibration_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_test_temperature_calibration_min());

    TEST_ASSERT_TRUE(set_temperature_calibration_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_test_temperature_calibration_max());

    TEST_ASSERT_TRUE(set_humidity_calibration_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_test_humidity_calibration_min());

    TEST_ASSERT_TRUE(set_humidity_calibration_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_test_humidity_calibration_max());

    TEST_ASSERT_TRUE(set_temperature_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_test_temperature_target_min());

    TEST_ASSERT_TRUE(set_temperature_target_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_test_temperature_target_max());

    TEST_ASSERT_TRUE(set_humidity_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_test_humidity_target_min());

    TEST_ASSERT_TRUE(set_humidity_target_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_test_humidity_target_max());

    TEST_ASSERT_TRUE(set_light_intensity_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_test_light_intensity_target_min());

    TEST_ASSERT_TRUE(set_water_level_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_test_water_level_target_min());

    TEST_ASSERT_TRUE(set_water_level_target_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_test_water_level_target_max());

    TEST_ASSERT_TRUE(set_soil_moisture_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_test_soil_moisture_target_min());

    TEST_ASSERT_TRUE(set_test_humidity_calibration_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_humidity_calibration_min());

    TEST_ASSERT_TRUE(set_test_humidity_calibration_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_humidity_calibration_max());

    TEST_ASSERT_TRUE(set_soil_moisture_target_max(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_test_soil_moisture_target_max());

    TEST_ASSERT_TRUE(set_water_volume_calibration_max(600));
    TEST_ASSERT_EQUAL_UINT16(600, get_test_water_volume_calibration_max());

    TEST_ASSERT_TRUE(set_test_soil_moisture_sensor_status(OKAY));
    TEST_ASSERT_EQUAL_UINT16(OKAY, get_soil_moisture_sensor_status());
}

void test_sdcard(void)
{
    /* Test for different cases of sdcard mimic usage */

    reset_sdcard();
    sdcard_create_file("33");
    TEST_ASSERT_EQUAL_UINT8(INVALID_FILE_NAME, sdcard.create_status);

    reset_sdcard();
    sdcard_create_file("07");
    TEST_ASSERT_EQUAL_UINT8(SUCCESS, sdcard.create_status);

    reset_sdcard();
    sdcard_create_file("19");
    TEST_ASSERT_EQUAL_UINT8(SUCCESS, sdcard.create_status);

    reset_sdcard();
    sdcard_append_file("32", "TEST APPEND");
    TEST_ASSERT_EQUAL_UINT8(INVALID_FILE_NAME, sdcard.append_status);

    char temp[10] = {};

    reset_sdcard();
    sdcard_read_file("02", temp, sizeof(temp));
    TEST_ASSERT_EQUAL_UINT8(FILE_NOT_EXIST, sdcard.read_status);

    reset_sdcard();
    sdcard_read_file("35", temp, sizeof(temp));
    TEST_ASSERT_EQUAL_UINT8(INVALID_FILE_NAME, sdcard.read_status);

    reset_sdcard();
    sdcard_read_file("18", temp, sizeof(temp));
    TEST_ASSERT_EQUAL_UINT8(SUCCESS, sdcard.read_status);
    TEST_ASSERT_EQUAL_STRING(temp, "[2020-04-");

    reset_sdcard();
    TEST_ASSERT_EQUAL_UINT8(SUCCESS, sdcard_read_file("19", temp, sizeof(temp)));
    TEST_ASSERT_EQUAL_STRING(temp, "");

    reset_sdcard();
    sdcard.read_status = READ_FILE_ERROR;
    sdcard_read_file("19", temp, sizeof(temp));
    TEST_ASSERT_EQUAL_UINT8(READ_FILE_ERROR, sdcard.read_status);
    TEST_ASSERT_EQUAL_STRING(temp, "");

    reset_sdcard();
    sdcard_delete_file("07");
    TEST_ASSERT_EQUAL_UINT8(SUCCESS, sdcard.delete_status);
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    delay(3000);
#else
int main()
{
#endif
    UNITY_BEGIN();

    RUN_TEST(test_log_manager_init);
    RUN_TEST(test_candata);
    RUN_TEST(test_log_manager_run);
    RUN_TEST(test_sdcard);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}