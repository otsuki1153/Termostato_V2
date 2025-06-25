#include <Arduino.h>
#include "Termo_Control.h"

TERMOCONTROLL termo;

void TERMOCONTROLL::setTemp(float temp){
    this->displayTemp = temp;
}

float TERMOCONTROLL::getTemp(){
    return displayTemp;
}


void TERMOCONTROLL::setSetPoint(float setPoint){
    this->setPointValue = setPoint;
}

float TERMOCONTROLL::getSetPoint(){
    return setPointValue;
}

void TERMOCONTROLL::setPowerTermo(bool power){
    this->power = power;
}

bool TERMOCONTROLL::getPowerTermo(){
    return power;
}
