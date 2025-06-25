#pragma once
#include <Arduino.h>

#define DEBUG 1
#define DEBUG_CONSUMPTION 0
#define DEBUG_DISPLAY 0

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#define ONE_HOUR 3600000
#define FIFTEEN_MINUTES 900000
#define TEN_MINUTES 600000
#define FIVE_MINUTES 300000
#define ONE_MINUTE 60000

#define BUTTONS_DEBOUNCE_TIME 150

extern TaskHandle_t blinkTaskHandle;
extern TaskHandle_t wifiConfigTaskHandle;
extern TaskHandle_t eventTaskHandle; 
extern TaskHandle_t updateStatusTaskHandle;

/**
 * @brief Handles button logic for locking other button functionalities, 
 *        and unlocking them when required.
 */
void lockButtons();

/**
 * @brief Handles button logic for setting the temperature on the thermostat.
 */
void setpointButtons();

/**
 * @brief Handles button logic for configuring Wi-Fi or turning the device on/off.
 */
void bothButtons();

/**
 * @brief Handles button logic for resetting the device.
 */
void resetDevice();