#pragma once

class GPIOS{
    private:
    #define RED_PIN 26
    #define GREEN_PIN 27
    #define BLUE_PIN 14

    #define CHANNEL_RED_PIN 1
    #define CHANNEL_GREEN_PIN 2
    #define CHANNEL_BLUE_PIN 3
    

    #define LED_BUTTONS 16 // horizontal 15
    #define RESET_BUTTON 25 // 
    
    #define DOWN_BUTTON 17 // horizontal 19
    #define UP_BUTTON 5 //
    #define RELAY 4 // 
    #define WTD_PIN 12 //
    
    #define LDR_PIN 36 //
    #define NTC_PIN 33
    #define FLOOR_SENSOR_PIN 39
    #define VREF_PIN 32

    public:

    /// @brief Função que dá pinMode() em todos os pinos do Dispositivo(801,810). -------
    /// @brief OBSERVAÇÃO: Deve ser colocado na função setup
    void init();
}; 
extern GPIOS gpio;