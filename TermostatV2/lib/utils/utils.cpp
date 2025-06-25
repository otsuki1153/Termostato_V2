#include <utils.h>
#include <main.h>
#include <gpios.h>
#include <eeprom_addr.h> 
#include <WiFi.h>

// uint8_t lockedTries = 0;
unsigned long buttonPressedTime = 0;
TaskHandle_t resetTaskHandle = NULL;

void lockButtons(){

  if(digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON)){
    delay(BUTTONS_DEBOUNCE_TIME);
    if(digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON)){
      buttonPressedTime = millis();
      bool finished = false;
      uint8_t progress = 0;
      // lockedTries--;
      while(digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON)){
        progress = map(millis()-buttonPressedTime, 0, 3960, 0, 12);
        Serial.println(progress);
        if(progress >= 12){
          delay(330);
          if(updateStatusTaskHandle != NULL){
            if(eTaskGetState(updateStatusTaskHandle) != eRunning){
              vTaskResume(updateStatusTaskHandle);
            }
          }
          delay(1000);
          // lockedTries = 0;
          finished = true;
          break;
        }
        delay(10);
      }
      if(finished){
        while(digitalRead(UP_BUTTON) && digitalRead(DOWN_BUTTON)) delay(10);
      } else {
        while(progress){
          progress--;
          if(progress < 0) progress = 0;
          Serial.println(progress);
          delay(50);
        }
      }
    }
  }

  if((digitalRead(DOWN_BUTTON) && !digitalRead(UP_BUTTON)) || (digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON))){
    delay(BUTTONS_DEBOUNCE_TIME);
    if((digitalRead(DOWN_BUTTON) && !digitalRead(UP_BUTTON)) || (digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON))){


      while((digitalRead(DOWN_BUTTON) && !digitalRead(UP_BUTTON)) || (digitalRead(UP_BUTTON) && !digitalRead(DOWN_BUTTON))){
        Serial.print(".");
        delay(100);
      }
      buttonPressedTime = millis();
      // lockedTries++;
      // Serial.println(lockedTries);
    
      // if(lockedTries >= 3){
      //   Serial.println("Notificação tentativas com cadeado!");

      //   // String jsonNotifications = "{\"alert\":\"lockedTries\",\"value\":true}";
      //   aws.setNextMsg((char*)("{\"alert\":\"lockedTries\",\"value\":true}"), aws.getDeviceId(true)+"/notifications");
      //   device.showLockFeedback = true;
      //   device.setScreen(LOCK);
      //   // notifica
      //   // seta animação
      //   lockedTries = 0;
      //   delay(1000);
      // }
    }
  }

  // if(lockedTries && millis()-buttonPressedTime >= 60000){
  //   lockedTries = 0;
  //   Serial.println("zerou contagem");
  // }
}

void setpointButtons(){
  bool upButton = digitalRead(UP_BUTTON), downButton = digitalRead(DOWN_BUTTON), resetButton = digitalRead(RESET_BUTTON);
 
}

void bothButtons() {
  bool upButton = digitalRead(UP_BUTTON), downButton = digitalRead(DOWN_BUTTON), resetButton = digitalRead(RESET_BUTTON);
 
}

void resetDevice(){
  if(resetTaskHandle == NULL){
    detachInterrupt(digitalPinToInterrupt(RESET_BUTTON));
    xTaskCreate([](void * pv){
      Serial.println("Reset Device");
      int countTimer = 0;
      uint32_t millisAction = millis();
      while(digitalRead(RESET_BUTTON) && countTimer < 100 && millis() - millisAction < 2000){
        if(countTimer == 30){
          if(blinkTaskHandle != NULL)
            vTaskResume(blinkTaskHandle);
        }
        countTimer++;
        millisAction = millis();
        delay(100);
      }

      if(blinkTaskHandle != NULL){
        vTaskSuspend(blinkTaskHandle);
      }

      if(countTimer == 100){
        bool result = false;
    

        if(result){
          EEPROM.write(FIRST_SETUP, 0);
          EEPROM.write(FIRST_SETUP_TIME_ON, 0);
          EEPROM.commit();
        } else {
          EEPROM.writeBool(RESET_ADDR, 1);
          EEPROM.commit();
        }
      }else{
        attachInterrupt(digitalPinToInterrupt(RESET_BUTTON), resetDevice, RISING);
        resetTaskHandle = NULL;
        vTaskDelete(NULL);
      }
    }, "ResetDevice", 1024*4, NULL, 2, &resetTaskHandle);
  }
}
