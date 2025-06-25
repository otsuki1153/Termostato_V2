#pragma once
#include <Arduino.h>
#include <TM1637TinyDisplay.h>

#define CLK 18
#define DIO 23

class Display {
  public:
    /**
     * @brief Show String
     * 
     * @param bright 
     */
    void showText(const char* string, uint8_t scrollDelay, uint8_t bright);

    void showLockAnimation(uint8_t progress, uint8_t bright);
    /**
     * @brief Show temperature set on display
     * 
     * @param bright 
     */
    void showSetpointAnimation(float setpoint, uint8_t bright, bool fahrenheit = false);
    /**
     * @brief Shows temperature on display I2C
     * 
     * @param temp 
     * @param bright 
     * @param leadingZero 
     * @return true 
     * @return false 
     */
    bool showTemp(float temp, uint8_t bright, bool leadingZero, bool fahrenheit = false);
    /**
     * @brief Show trace on display I2C
     * 
     * @param state (Show or Clear)
     * @param bright (Bright display)
     */
    void showTrace(bool state, uint8_t bright);
    /**
     * @brief Show ON or OFF on display I2C
     * 
     * @param power (On or OFF)
     * @param bright (Bright display)
     */
    void showPower(bool power, uint8_t bright);
    /**
     * @brief Show config on display I2C
     * 
     * @param configMode (CONFIG, SMART_CONFIG, AP_CONFIG)
     * @param bright (Bright display)
     */
    void showConfig(bool apMode, uint8_t bright);
    /**
     * @brief Show error on display I2C
     * 
     * @param bright (Bright display)
     */
    void showError(uint8_t bright);
    /**
     * @brief Show text 'Smartly'
     * 
     * @param bright 
     */
    void showSmartly(int deviceGroup, uint8_t bright); 
    /**
     * @brief Show job animation on display I2C.
     * 
     * @param bright 
     */
    void job(uint8_t bright);
    /**
     * @brief Clear display I2C
     * 
     */
    void clear();
  private:
    void filterBright(uint8_t * bright);
};

extern Display display;