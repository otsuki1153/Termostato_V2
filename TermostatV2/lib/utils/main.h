#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

/**
 * @brief Connects the device to the internet and AWS Cloud.
 * 
 * @param pvParameters 
 */
void wifiConnection(void *pvParameters);

/**
 * @brief Reads temperature and luminosity sensors.
 * 
 * @param pvParameters 
 */
void sensorTask(void *pvParameters);

/**
 * @brief Updates display information.
 * 
 * @param pvParameters 
 */
void displayTask(void *pvParameters);

/**
 * @brief Controls RGB led.
 * 
 * @param pvParameters 
 */
void ledTask(void *pvParameters);

/**
 * @brief Blinks RGB led and Display.
 * 
 * @param pvParameters 
 */
void blinkTask(void *pvParameters);

/**
 * @brief Checks if buttons are being pressed and then executes its respective function
 * 
 * @param pvParameters 
 */
void verifyButtons(void *pvParameters);

/**
 * @brief Manages wifi configuration.
 * 
 * @param pvParameters 
 */
void wifiConfiguration(void *pvParameters);

/**
 * @brief Manages device events and tasks.
 * 
 * @param pvParameters 
 */
void eventHandle(void *pvParameters);

/**
 * @brief Downloads Job.
 * 
 * @param pvParameters 
 */
void processJob(void *pvParameters);

/**
 * @brief Verifies device changes and update device status.
 * 
 * @param pvParameters 
 */
void updateStatusTask(void *pvParameters);

/**
 * @brief Calculates consumption.
 * 
 * @param pvParameters 
 */
void calcConsumption(void *pvParameters);

/**
 * @brief Updates epoch time every 5 minutes.
 * 
 * @param pvParameters 
 */
void updateEpoch(void *pvParameters);

#endif