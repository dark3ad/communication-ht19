#ifndef CAN_DATA_H
#define CAN_DATA_H

#include <stdint.h>
#include <stdbool.h>

#define PAYLOADS_LENGTH (768U)

typedef struct
{
    uint8_t value;
    uint8_t status;
    struct
    {
        struct
        {
            uint8_t value;
            uint8_t valid;
        } min;
        struct
        {
            uint8_t value;
            uint8_t valid;
        } max;
    } calibration;
    struct
    {
        struct
        {
            uint8_t value;
            uint8_t valid;
        } min;
        struct
        {
            uint8_t value;
            uint8_t valid;
        } max;
    } target;
} temperature_t;

typedef struct
{
    uint8_t value;
    uint8_t status;
    struct
    {
        struct
        {
            uint8_t value;
            uint8_t valid;
        } min;
        struct
        {
            uint8_t value;
            uint8_t valid;
        } max;
    } calibration;
    struct
    {
        struct
        {
            uint8_t value;
            uint8_t valid;
        } min;
        struct
        {
            uint8_t value;
            uint8_t valid;
        } max;
    } target;
} humidity_t;

typedef struct
{
    uint8_t status;
    uint8_t intensity;
    struct
    {
        uint8_t value;
        uint8_t valid;
    } target;
} light_t;

typedef struct
{
    uint16_t value;
    uint8_t status;
} flow_meter_t;

typedef struct
{
    uint8_t value;
    uint8_t status;
    struct
    {
        struct
        {
            uint8_t value;
            uint8_t valid;
        } min;
        struct
        {
            uint8_t value;
            uint8_t valid;
        } max;
    } target;
} moisture_t;

typedef struct
{
    uint8_t value;
    uint8_t status;
    struct
    {
        struct
        {
            uint8_t value;
            uint8_t valid;
        } min;
        struct
        {
            uint8_t value;
            uint8_t valid;
        } max;
    } target;
} water_level_t;

typedef struct
{
    uint8_t status;
    struct
    {
        uint8_t value;
        uint8_t overwritten;
    } state;
} fans_t;

typedef struct
{
    uint8_t status;
    struct
    {
        uint8_t value;
        uint8_t overwritten;
    } shininess;
} lamp_t;

typedef struct
{
    uint8_t status;
    struct
    {
        uint8_t value;
        uint8_t overwritten;
    } warmness;
} heater_t;

typedef struct
{
    uint8_t status;
    struct
    {
        uint8_t value;
        uint8_t overwritten;
    } state;
} water_pump_t;

typedef struct
{
    uint8_t status;
    struct
    {
        uint8_t value;
        uint8_t overwritten;
    } state;
} water_valve_t;

typedef struct
{
    uint8_t status;
    struct
    {
        uint8_t value;
        uint8_t overwritten;
    } state;
} window_t;

typedef struct
{
    uint8_t status;
    char datetime[20];
} rtc_t;

typedef struct
{
    uint8_t status;
} esp32_t;

typedef struct
{
    uint8_t status;
    uint16_t free_space;
} sdcard_t;

typedef struct
{
    uint8_t status;
} terminal_t;

typedef struct
{
    uint8_t status;
} rgb_t;

typedef struct
{
    uint8_t status;
} keypad_t;

typedef struct
{
    uint8_t status;
} buzzer_t;

typedef struct
{
    uint8_t status;
} eeprom_t;

typedef struct
{
    uint8_t status;
} system_t;

typedef struct
{
    uint8_t status;
} display_t;

typedef struct
{
    temperature_t temperature;
    humidity_t humidity;
    light_t light;
    flow_meter_t flow_meter;
    moisture_t moisture;
    water_level_t water_level;
} sensor_t;

typedef struct
{
    fans_t fans;
    lamp_t lamp;
    heater_t heater;
    water_pump_t water_pump;
    water_valve_t water_valve;
    window_t window;
} actuator_t;

typedef struct
{
    rtc_t rtc;
    esp32_t esp32;
    sdcard_t sdcard;
    terminal_t terminal;
} communication_t;

typedef struct
{
    rgb_t rgb;
    keypad_t keypad;
    buzzer_t buzzer;
    eeprom_t eeprom;
    system_t system;
    display_t display;
} hmi_t;

typedef struct
{
    sensor_t sensor;
    actuator_t actuator;
    communication_t communication;
    hmi_t hmi;
} data_t;

data_t get_candata(void);

void get_payloads(char *payloads);

bool is_temperature_changed(temperature_t previous, temperature_t current);
void get_temperature_text(char *text, temperature_t temperature);

bool is_humidity_changed(humidity_t previous, humidity_t current);
void get_humidity_text(char *text, humidity_t humidity);

bool is_light_changed(light_t previous, light_t current);
void get_light_text(char *text, light_t light);

bool is_flow_meter_changed(flow_meter_t previous, flow_meter_t current);
void get_flow_meter_text(char *text, flow_meter_t flow_meter);

bool is_moisture_changed(moisture_t previous, moisture_t current);
void get_moisture_text(char *text, moisture_t moisture);

bool is_water_level_changed(water_level_t previous, water_level_t current);
void get_water_level_text(char *text, water_level_t water_level);

bool is_fans_changed(fans_t previous, fans_t current);
void get_fans_text(char *text, fans_t fans);

bool is_lamp_changed(lamp_t previous, lamp_t current);
void get_lamp_text(char *text, lamp_t lamp);

bool is_heater_changed(heater_t previous, heater_t current);
void get_heater_text(char *text, heater_t heater);

bool is_water_pump_changed(water_pump_t previous, water_pump_t current);
void get_water_pump_text(char *text, water_pump_t water_pump);

bool is_water_valve_changed(water_valve_t previous, water_valve_t current);
void get_water_valve_text(char *text, water_valve_t water_valve);

bool is_window_changed(window_t previous, window_t current);
void get_window_text(char *text, window_t window);

bool is_rtc_changed(rtc_t previous, rtc_t current);
void get_rtc_text(char *text, rtc_t rtc);

bool is_esp32_changed(esp32_t previous, esp32_t current);
void get_esp32_text(char *text, esp32_t esp32);

bool is_sdcard_changed(sdcard_t previous, sdcard_t current);
void get_sdcard_text(char *text, sdcard_t sdcard);

bool is_terminal_changed(terminal_t previous, terminal_t current);
void get_terminal_text(char *text, terminal_t terminal);

bool is_rgb_changed(rgb_t previous, rgb_t current);
void get_rgb_text(char *text, rgb_t rgb);

bool is_keypad_changed(keypad_t previous, keypad_t current);
void get_keypad_text(char *text, keypad_t keypad);

bool is_buzzer_changed(buzzer_t previous, buzzer_t current);
void get_buzzer_text(char *text, buzzer_t buzzer);

bool is_eeprom_changed(eeprom_t previous, eeprom_t current);
void get_eeprom_text(char *text, eeprom_t eeprom);

bool is_system_changed(system_t previous, system_t current);
void get_system_text(char *text, system_t system);

bool is_display_changed(display_t previous, display_t current);
void get_display_text(char *text, display_t display);

#endif /* CAN_DATA_H */