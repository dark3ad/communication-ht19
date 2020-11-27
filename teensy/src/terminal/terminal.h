/**
 * @author      : jesper (jesper@jesper-HP-ENVY-Laptop-13-aq0xxx)
 * @file        : terminal
 * @created     : tisdag nov 10, 2020 13:26:45 CET
 */

#ifndef TERMINAL_H

#define TERMINAL_H

#define READ_BUFFER_SIZE 64

#define MAIN_MENU_SNAPSHOT 0
#define MAIN_MENU_FILES 1
#define MAIN_MENU_CALIBRATION 2

#define CALIBRATION_MENU_TEMPERATURE 0
#define CALIBRATION_MENU_HUMIDITY 1
#define CALIBRATION_MENU_FLOW_METER 2
#define CALIBRATION_MENU_WATER_LEVEL 3

#define TEMP_MENU_SET_TARGET_MIN 0
#define TEMP_MENU_SET_TARGET_MAX 1
#define TEMP_MENU_SET_CALI_MIN 2
#define TEMP_MENU_SET_CALI_MAX 3

#define HUM_MENU_SET_TARGET_MIN 0
#define HUM_MENU_SET_TARGET_MAX 1
#define HUM_MENU_SET_CALI_MIN 2
#define HUM_MENU_SET_CALI_MAX 3

#define FLOW_MENU_SET_CALI_MAX 0

#define MOIS_MENU_SET_TARGET_MIN 0
#define MOIS_MENU_SET_TARGET_MAX 1

#define WATER_LEVEL_MENU_TARGET_MIN 0
#define WATER_LEVEL_MENU_TARGET_MAX 1
#define WATER_LEVEL_MENU_CAL_MAX 2

#define LIGHT_MENU_TARGET_MIN 0

int terminal_initialize(void);
int terminal_run(void);


#endif /* end of include guard TERMINAL_H */

