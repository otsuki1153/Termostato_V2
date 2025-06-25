#include "display_lib.h"
#include "gpios.h"
#include "Termo_Control.h"

TM1637TinyDisplay dis(CLK, DIO);
Display display;

void Display::showText(const char *s, uint8_t scrollDelay, uint8_t bright){
  if(bright > 4)
    bright = 4;
  else if(bright < 0)
    bright = 1;

  // bool on = bright;
  filterBright(&bright);
  dis.setBrightness(bright);
  dis.setScrolldelay(scrollDelay);
  dis.showString(s, 4, 0);
}

void Display::showLockAnimation(uint8_t progress, uint8_t bright){
  if(bright > 4)
    bright = 4;
  else if(bright < 0)
    bright = 1;

  filterBright(&bright);
  dis.setBrightness(bright);



    
    uint8_t segmentos[4] = {0b00000000, 0b00000000, 0b00000000, 0b00000000};
}

void Display::showSetpointAnimation(float setpoint, uint8_t bright, bool fahrenheit){
  int ten, unit, decimal, hundred;
  String measureUnit = "C";

  if (fahrenheit) {
    setpoint = (setpoint * 9/5) + 32;
    measureUnit = "F";
  }

  if (setpoint >= 100) {
    hundred = setpoint / 100;
    ten = (setpoint - (hundred * 100)) / 10;
    unit = setpoint - (hundred * 100) - (ten * 10);
  } else {
    hundred = 0;
    ten = setpoint / 10;
    unit = setpoint - (ten * 10);
    decimal = (setpoint * 10) - ((ten * 100) + (unit * 10));
  }

  if (bright > 4)
    bright = 4;
  else if (bright < 0)
    bright = 1;

  filterBright(&bright);
  dis.setBrightness(bright);
  dis.setScrolldelay(150);
  dis.showString(" SET", 4, 0);
  if(termo.getSetPoint() > termo.getTemp()){
    digitalWrite(RELAY, HIGH);
    ledcWrite(CHANNEL_RED_PIN, 0);
    ledcWrite(CHANNEL_BLUE_PIN, 4096);
    ledcWrite(CHANNEL_GREEN_PIN, 4096);
  } else if(termo.getSetPoint() < termo.getTemp()){
    digitalWrite(RELAY, LOW);
    ledcWrite(CHANNEL_RED_PIN, 4096);
    ledcWrite(CHANNEL_BLUE_PIN, 4096); 
    ledcWrite(CHANNEL_GREEN_PIN, 4096);
  }
  delay(1000);

  for (int i = 1; i < 5; i++) {
    if(fahrenheit){
      if(setpoint >= 100)
        dis.showNumberDec(hundred, 0, 0, 1, 0);
      else dis.showString(" ", 1, 0);
      
      dis.showNumberDec(ten, 0, 0, 1, 1);
      dis.showNumberDec(unit, 0, 0, 1, 2);
    } else {
      dis.showNumberDec(ten, 0, 0, 1, 0);
      dis.showNumberDec(unit, 0b11100000, 0, 1, 1);
      dis.showNumberDec(decimal, 0, 0, 1, 2);
    }
    dis.showString(measureUnit.c_str(), 1, 3);
    delay(300);
    dis.clear();
    delay(300);
  }
  delay(500);
}

bool Display::showTemp(float temp, uint8_t bright, bool leadingZero, bool fahrenheit){
  int ten, unit, decimal, hundred;
  String measureUnit = "C";

  if (fahrenheit) {
    temp = (temp * 9/5) + 32;
    measureUnit = "F";
  }

  if (temp >= 100) {
    hundred = temp / 100;
    ten = (temp - (hundred * 100)) / 10;
    unit = temp - (hundred * 100) - (ten * 10);
  } else {
    hundred = 0;
    ten = temp / 10;
    unit = temp - (ten * 10);
    decimal = (temp * 10) - ((ten * 100) + (unit * 10));
  }

  if(temp < 0 || temp >= 150){
    dis.setBrightness(15);
    dis.showString("Erro");
    return false;
  }

  if(bright > 4)
    bright = 4;
  else if(bright < 0)
    bright = 1;

  bool on = bright;
  filterBright(&bright);
  dis.setBrightness(bright, on);

  if(fahrenheit){
    if(temp >= 100)
      dis.showNumberDec(hundred, 0, leadingZero, 1, 0);
    else dis.showString(" ", 1, 0);
    
    dis.showNumberDec(ten, 0, leadingZero, 1, 1);
    dis.showNumberDec(unit, 0, leadingZero, 1, 2);
    dis.showString(measureUnit.c_str(), 1, 3);
  } else {
    dis.showNumberDec(ten, 0, leadingZero, 1, 0);
    dis.showNumberDec(unit, 0b11100000, leadingZero, 1, 1);
    dis.showNumberDec(decimal, 0, leadingZero, 1, 2);
    dis.showString(measureUnit.c_str(), 1, 3);
  }

  return true;
}

void Display::showTrace(bool state, uint8_t bright){
  if(bright > 4)
    bright = 4;
  else if(bright < 0)
    bright = 1;

  // bool on = bright;
  filterBright(&bright);
  dis.setBrightness(bright);

  if(state)
    dis.showString("----");
  else
    dis.clear();
}

void Display::showPower(bool power, uint8_t bright){
  if(bright > 4)
    bright = 4;
  else if(bright < 0)
    bright = 1;

  bool on = bright;
  filterBright(&bright);
  dis.setBrightness(bright, on);

  if(on){
    if(power)
      dis.showString(" On ", 4, 0);
    else
      dis.showString(" Off", 4, 0);
  } else dis.clear();
}

void Display::showConfig(bool apMode, uint8_t bright){
  if(bright > 4)
    bright = 4;
  else if(bright < 0)
    bright = 1;

  // bool on = bright;
  filterBright(&bright);
  dis.setBrightness(bright);

  apMode ? dis.showString("Con2") : dis.showString("ConF");
}

void Display::showError(uint8_t bright){
  if(bright > 4)
    bright = 4;
  else if(bright < 0)
    bright = 1;

  // bool on = bright;
  filterBright(&bright);
  dis.setBrightness(bright);

  dis.setBrightness(bright);

  dis.showString("Erro");
}

void Display::showSmartly(int deviceGroup, uint8_t bright){
  if(bright > 4)
    bright = 4;
  else if(bright < 0)
    bright = 1;

  // bool on = bright;
  filterBright(&bright);
  dis.setBrightness(bright);
  dis.clear();

  dis.setScrolldelay(350);

  if(deviceGroup)
    dis.showString("HF900");
  else dis.showString("HF810");
}

void Display::job(uint8_t bright){
  if(bright > 4)
    bright = 4;
  else if(bright < 0)
    bright = 1;

  // bool on = bright;
  filterBright(&bright);
  dis.setBrightness(bright);
  dis.clear();

  dis.setScrolldelay(250);

  while(1){
    dis.showString("-   ");
    delay(250);
    dis.showString(" -  ");
    delay(250);
    dis.showString("  - ");
    delay(250);
    dis.showString("   -");
    delay(250);
    dis.showString("    ");
    delay(250);
  }
}

void Display::clear(){
  dis.clear();
}

void Display::filterBright(uint8_t * bright){
  switch (*bright){
  case 0:
    *bright = BRIGHT_LOW;
    break;
  case 1:
    *bright = BRIGHT_0;
    break;
  case 2:
    *bright = BRIGHT_1;
    break;
  case 3:
    *bright = BRIGHT_3;
    break;
  case 4:
    *bright = BRIGHT_HIGH;
    break;
  default:
    *bright = BRIGHT_HIGH;
    break;
  }
}
