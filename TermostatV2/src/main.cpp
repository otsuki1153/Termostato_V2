#include <Arduino.h>
#include "display_lib.h"
#include "ntc_sensor.h"
#include "gpios.h"
#include "main_tasks.h"
#include "main_var.h"
#include "Termo_Control.h"

//=================================================== SETUP =======================================================================
void setup() {
  Serial.begin(115200);
  gpio.init();
  ntc.init();

  termo.setPowerTermo(true);
  // termo.setTemp(16.0);

  // displayTemp = ntc.hasFloorSensor() ? ntc.getFloorTemp() : ntc.getEnvTemp();
 

  xTaskCreate(displayTask, "displayTask", 4096, NULL, 1, &DisplayTask);
  xTaskCreate(buttonTask, "buttonTask", 4096, NULL, 1, &ButtonTask);
  xTaskCreate(relayControlTask, "relayControlTask", 4096, NULL, 1, &RelayTask);
  xTaskCreate(brightControll, "brightControll", 4096, NULL, 1, &BrightControll);
}
//-----------------------------------------------------------------------------------------------------------------------------------

//=================================================== TASK LOOP (Cuida da Leitura da Temperatura) =====================================================================
void loop() {

  if(ntc.hasFloorSensor()){
    ntc.calculateTemperature(true);
    ntc.calculateTemperature(false);
  } else {
    ntc.calculateTemperature(false);
  }

  if(ntc.hasSensorError()){
    Serial.print("error");
    error = true;
  } else{
    error = false;
  }
  
  termo.setTemp(ntc.hasFloorSensor() ? ntc.getFloorTemp() : ntc.getEnvTemp());
  delay(1000);
}
//-----------------------------------------------------------------------------------------------------------------------------------

void brightControll(void * pv){
  while(1){
    displayBright = displayBright >= 4 ? 4 : displayBright;
    avaregeRead = 0;

    for(int i = 0; i <= 50; i++){
      avaregeRead += analogRead(LDR_PIN);
    }

    finalAvaregeRead = avaregeRead/50;

    if((finalAvaregeRead <= 4096 || finalAvaregeRead >= 4096) && finalAvaregeRead >= 1000){
      displayBright = 3;
      lowerLedBright = 1000;
    } else if(finalAvaregeRead <= 999 && finalAvaregeRead >= 251){
      displayBright = 2;
      lowerLedBright = 3000;
    } else if(finalAvaregeRead <= 250 && finalAvaregeRead >= 0){
      displayBright = 1;
      lowerLedBright = 4000;
    }

    delay(2000);
  }
  
}

//=================================================== TASK Temperatura e Relay =====================================================================
void relayControlTask(void * pv){
  while (1){
    // if(millis() - RelayTime >= 2000){
      // RelayTime = millis();
      if(!error){
        if((termo.getSetPoint() > termo.getTemp()) && termo.getPowerTermo() && !setPointMode && !smartInitAnimationDisplay && !showtraceAnimDisplay){
          digitalWrite(RELAY, HIGH);
          ledcWrite(CHANNEL_RED_PIN, lowerLedBright);
          ledcWrite(CHANNEL_BLUE_PIN, 4096);
          ledcWrite(CHANNEL_GREEN_PIN, 4096);
        } else if((termo.getSetPoint() < termo.getTemp()) && termo.getPowerTermo() && !setPointMode && !smartInitAnimationDisplay && !showtraceAnimDisplay){
          digitalWrite(RELAY, LOW);
          ledcWrite(CHANNEL_RED_PIN, 4096);
          ledcWrite(CHANNEL_BLUE_PIN, 4096);
          ledcWrite(CHANNEL_GREEN_PIN, 4096);
        } else if(!termo.getPowerTermo() && !showtraceAnimDisplay){
          digitalWrite(RELAY, LOW);
          ledcWrite(CHANNEL_RED_PIN, 4096);
          ledcWrite(CHANNEL_BLUE_PIN, 4096);
          ledcWrite(CHANNEL_GREEN_PIN, 4096);
        }
      } else{
        digitalWrite(RELAY, LOW);
        while(error){  
          delay(500);    
          ledcWrite(CHANNEL_RED_PIN, 4096);
          ledcWrite(CHANNEL_BLUE_PIN, 4096);
          ledcWrite(CHANNEL_GREEN_PIN, 4096);
          delay(500);
          ledcWrite(CHANNEL_RED_PIN, 0);
          ledcWrite(CHANNEL_BLUE_PIN, 4096);
          ledcWrite(CHANNEL_GREEN_PIN, 0);
        }
      }
    // }
    delay(2);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

//=================================================== FUNÇÃO DA TASK DISPLAY ========================================================
void displayTask(void * pv){

  while (1){
    if(termo.getPowerTermo()){
      if(!error){
        if(smartInitAnimationDisplay){
          display.showSmartly(0, 4);
          display.showSetpointAnimation(termo.getSetPoint(), 4, false);
          smartInitAnimationDisplay = false;
        } else{
          if(!showtraceAnimDisplay && !showNextStatusDisplay){
            if(!setPointMode){
              display.showTemp(termo.getTemp(), displayBright, false, false);
            } else if(setPointMode && !setBlinkAnim){
              display.showTemp(setPointValueMain, 4, false, false);
            } else if(setPointMode && setBlinkAnim){
              display.showSetpointAnimation(termo.getSetPoint(), 4, false);
            }
          } else if(!showtraceAnimDisplay && showNextStatusDisplay){
            display.showPower(!termo.getPowerTermo(), 4);
          }else if(showtraceAnimDisplay && !showNextStatusDisplay){
            display.showTrace(true, 4);
            ledcWrite(CHANNEL_RED_PIN, 0);
            ledcWrite(CHANNEL_BLUE_PIN, 0);
            ledcWrite(CHANNEL_GREEN_PIN, 0);
            delay(250);
            display.showTrace(false, 4);
            digitalWrite(RED_PIN, HIGH);
            digitalWrite(BLUE_PIN, HIGH);
            digitalWrite(GREEN_PIN, HIGH);
            delay(250);
          }
        }
      } else{
        display.showError(4);
      }
    } else{
      if(!showtraceAnimDisplay && !showNextStatusDisplay){
        if((!digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON))){
          display.clear();
        } else if((digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON)) || (!digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON))){
          display.showPower(termo.getPowerTermo(), 4);
        }
      }else if(!showtraceAnimDisplay && showNextStatusDisplay){
        display.showPower(!termo.getPowerTermo(), 4);
      } else if(showtraceAnimDisplay && !showNextStatusDisplay){
        display.showTrace(true, 4);
        digitalWrite(RED_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        delay(250);
        display.showTrace(false, 4);
        ledcWrite(CHANNEL_RED_PIN, 4096);
        ledcWrite(CHANNEL_BLUE_PIN, 4096);
        ledcWrite(CHANNEL_GREEN_PIN, 4096);
        delay(250);
      }
    }
    delay(10);
  }
  
}
//-----------------------------------------------------------------------------------------------------------------------------------

//=================================================== FUNÇÃO DA TASK DOS BOTÕES =====================================================
void buttonTask(void * pv){
  
  termo.setSetPoint(16.0);
  setPointValueMain = termo.getSetPoint();

  while (1){

    if(ntc.hasFloorSensor()){
      setPointLimit = 35.0;
    } else if(!ntc.hasFloorSensor()){
      setPointLimit = 30.0;
      // setPointValueMain = setPointValueMain >= 30.0 ? 30.0 : setPointValueMain;
    }

    if(((!digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON)) || (digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON))) && !setBlinkAnim && termo.getPowerTermo() && !smartInitAnimationDisplay && !error){
      setPointMode = true;
      if(millis() - SetModeTime >= 125){
        delay(125);
        if(digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON)){
          SetModeTime = millis();
          setPointValueMain += setPointValueMain < setPointLimit ? 0.5 : 0;
          delay(125);
        }else if(!digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON)){
          SetModeTime = millis();
          setPointValueMain -= setPointValueMain <= 5.0 ? 0 : 0.5;
          delay(125);
        }
      }
    } else if((digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON)) && !termo.getPowerTermo() && !smartInitAnimationDisplay){
      if(millis() - SetModeTime >= 2000){
        SetModeTime = millis();
        while(1){
          showtraceAnimDisplay = true;
          
          if((!digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON)) || (digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON)) || (!digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON))){
            SetModeTime = millis();
            delay(500);
            showtraceAnimDisplay = false;
            termo.setPowerTermo(false);
            break;
          }

          if(millis() - SetModeTime >= 3000){
            SetModeTime = millis();
            showtraceAnimDisplay = false;
            showNextStatusDisplay = true;
            delay(2000);
            showNextStatusDisplay = false;
            smartInitAnimationDisplay = true;
            termo.setPowerTermo(true);
            while ((digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON))){
              if(!digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON)){
                break;
              }
            }
            break;
          }
        }
        
      }

    } else if((digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON)) && termo.getPowerTermo() && !smartInitAnimationDisplay){
      setPointValueMain = termo.getSetPoint();
      setPointMode = false;
      if(millis() - SetModeTime >= 2000){
        SetModeTime = millis();
        while(1){
          showtraceAnimDisplay = true;

          if((!digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON)) || (digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON)) || (!digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON))){
            SetModeTime = millis();
            delay(500);
            showtraceAnimDisplay = false;
            termo.setPowerTermo(true);
            setPointMode = false;
            break;
          }

          if(millis() - SetModeTime >= 3000){
            SetModeTime = millis();
            showtraceAnimDisplay = false;
            showNextStatusDisplay = true;
            delay(2000);
            showNextStatusDisplay = false;
            termo.setPowerTermo(false);
            while ((digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON))){
              if(!digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON)){
                break;
              }
            }
            break;
          }
        }
        
      }

    }


    if(setPointValueMain != termo.getSetPoint()){
      if((!digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON)) && setPointMode && !smartInitAnimationDisplay && !error){
        if(millis() - SetModeTime >= 2000){
          SetModeTime = millis();
          termo.setSetPoint(setPointValueMain);
          setBlinkAnim = true;
          delay(1000);
          setPointMode =  false;
          setBlinkAnim = false;
        }
      }
    } else{
      if((!digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON)) && setPointMode && !smartInitAnimationDisplay && !error){
        if(millis() - SetModeTime >= 1500){
          SetModeTime = millis();
          delay(1000);
          setPointMode = false;
        }

      }
    }
    delay(10);
  }
  
}
//-----------------------------------------------------------------------------------------------------------------------------------