#include <Arduino.h>
#include "gpios.h"

GPIOS gpio;


void GPIOS::init(){
    pinMode(WTD_PIN, OUTPUT);
    digitalWrite(WTD_PIN, HIGH);

    pinMode(LED_BUTTONS, OUTPUT);
    pinMode(RELAY, OUTPUT);

    ledcAttachPin(RED_PIN, CHANNEL_RED_PIN);
    ledcSetup(CHANNEL_RED_PIN, 1000, 12);


    ledcAttachPin(GREEN_PIN, CHANNEL_GREEN_PIN);
    ledcSetup(CHANNEL_GREEN_PIN, 1000, 12);


    ledcAttachPin(BLUE_PIN, CHANNEL_BLUE_PIN);
    ledcSetup(CHANNEL_BLUE_PIN, 1000, 12);

    //pinMode(RED_PIN, OUTPUT);
    //pinMode(GREEN_PIN, OUTPUT);
    //pinMode(BLUE_PIN, OUTPUT);
    
    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);
    pinMode(RESET_BUTTON, INPUT);
    pinMode(LDR_PIN, INPUT);
    pinMode(NTC_PIN, INPUT);
    pinMode(FLOOR_SENSOR_PIN, INPUT);
    pinMode(VREF_PIN,INPUT);
}