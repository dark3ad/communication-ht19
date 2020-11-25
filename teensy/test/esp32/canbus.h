#ifndef CANBUS_H
#define CANBUS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Get the RTC status.
 *
 * @return uint8_t Returns UNINITIALIZED or OKAY.
 */
uint8_t get_rtc_status(void);

/**
 * @brief Set the esp32 status.
 *
 * @param value Acceptable values: UNINITIALIZED, OKAY, I2C_ERROR, NTP_ERROR, WIFI_DISCONNECTED, MQTT_DISCONNECTED or MQTT_PUBLISH_ERROR.
 * @return true if the value is valid
 * @return false if the value is not valid
 */
bool set_esp32_status(uint8_t value);

/**
 * @brief Set the RTC status.
 *
 * @param value Acceptable values: UNINITIALIZED or OKAY.
 * @return true if the value is valid
 * @return false if the value is not valid
 */
bool set_rtc_status(uint8_t value);

/**
 * @brief Set the current RTC year.
 *
 * @param value A number between 2020 and 2040.
 * @return true if the value is valid
 * @return false if the value is not valid
 */
bool set_rtc_year(uint16_t value);

/**
 * @brief Set the current RTC month.
 *
 * @param value A number between 1 and 12.
 * @return true if the value is valid
 * @return false if the value is not valid
 */
bool set_rtc_month(uint8_t value);

/**
 * @brief Set the current RTC day.
 *
 * @param value A number between 1 and 31.
 * @return true if the value is valid
 * @return false if the value is not valid
 */
bool set_rtc_day(uint8_t value);

/**
 * @brief Set the current RTC hour.
 *
 * @param value A number between 0 and 23.
 * @return true if the value is valid
 * @return false if the value is not valid
 */
bool set_rtc_hour(uint8_t value);

/**
 * @brief Set the current RTC minute.
 *
 * @param value A number between 0 and 59.
 * @return true if the value is valid
 * @return false if the value is not valid
 */
bool set_rtc_minute(uint8_t value);

/**
 * @brief Set the current RTC second.
 *
 * @param value A number between 0 and 59.
 * @return true if the value is valid
 * @return false if the value is not valid
 */
bool set_rtc_second(uint8_t value);

#endif /* CANBUS_H */