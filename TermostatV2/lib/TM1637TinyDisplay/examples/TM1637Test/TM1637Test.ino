//  TM1637TinyDisplay TEST Sketch
//  This is a test sketch for the Arduino TM1637TinyDisplay LED Display library
//
//  Author: Jason A. Cox - @jasonacox - https://github.com/jasonacox
//  Date: 2 July 2020
//
//  Based on TM1637Display library at https://github.com/avishorp/TM1637
//

// Includes
#include <Arduino.h>
#include <TM1637TinyDisplay.h>

// Module connection pins (Digital Pins)
#define CLK 4
#define DIO 5

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   1000

// Example of manually defining a display pattern
const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

// Example animation sequence for showAnimation() Test
//  Built with 7-Segment Animator Tool
//  https://jasonacox.github.io/TM1637TinyDisplay/examples/7-segment-animator.html

const uint8_t ANIMATION[12][4] = {
  { 0x08, 0x00, 0x00, 0x00 },  // Frame 0
  { 0x00, 0x08, 0x00, 0x00 },  // Frame 1
  { 0x00, 0x00, 0x08, 0x00 },  // Frame 2
  { 0x00, 0x00, 0x00, 0x08 },  // Frame 3
  { 0x00, 0x00, 0x00, 0x04 },  // Frame 4
  { 0x00, 0x00, 0x00, 0x02 },  // Frame 5
  { 0x00, 0x00, 0x00, 0x01 },  // Frame 6
  { 0x00, 0x00, 0x01, 0x00 },  // Frame 7
  { 0x00, 0x01, 0x00, 0x00 },  // Frame 8
  { 0x01, 0x00, 0x00, 0x00 },  // Frame 9
  { 0x20, 0x00, 0x00, 0x00 },  // Frame 10
  { 0x10, 0x00, 0x00, 0x00 }   // Frame 11
};

TM1637TinyDisplay display(CLK, DIO);

void setup()
{
}

void loop()
{
  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };    // Test Pattern - All
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };   // Test Pattern - Blank
  display.setBrightness(BRIGHT_HIGH);

  // All segments on
  display.setSegments(data);
  delay(TEST_DELAY);

  // Test setting different digits
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);
  display.setSegments(data);
  delay(TEST_DELAY);

  // Test display splitting with position
  display.clear();
  display.setSegments(data + 2, 2, 2);    // Length 2, Position 2
  delay(TEST_DELAY);
  display.clear();
  display.setSegments(data + 2, 2, 1);    // Length 2, Position 1
  delay(TEST_DELAY);
  display.clear();
  display.setSegments(data + 1, 3, 1);    // Length 3, Position 1
  delay(TEST_DELAY);

  // Test decimal numbers with/without leading zeros in different positions
  display.showNumber(0, false);         // Expect: ___0
  delay(TEST_DELAY);
  display.showNumber(0, true);          // Expect: 0000
  delay(TEST_DELAY);
  display.showNumber(1, false);         // Expect: ___1
  delay(TEST_DELAY);
  display.showNumber(1, true);          // Expect: 0001
  delay(TEST_DELAY);
  display.showNumber(301, false);       // Expect: _301
  delay(TEST_DELAY);
  display.showNumber(301, true);        // Expect: 0301
  delay(TEST_DELAY);
  display.clear();
  display.showNumber(14, false, 2, 1);  // Expect: _14_
  delay(TEST_DELAY);
  display.clear();
  display.showNumber(4, true, 2, 2);    // Expect: __04
  delay(TEST_DELAY);
  display.showNumber(-1, false);        // Expect: __-1
  delay(TEST_DELAY);
  display.showNumber(-12);              // Expect: _-12
  delay(TEST_DELAY);
  display.showNumber(-999);             // Expect: -999
  delay(TEST_DELAY);
  display.clear();
  display.showNumber(-5, false, 3, 0);  // Expect: _-5_
  delay(TEST_DELAY);
  display.showNumberHex(0xf1af);        // Expect: f1Af
  delay(TEST_DELAY);
  display.showNumberHex(0x2c);          // Expect: __2C
  delay(TEST_DELAY);
  display.showNumberHex(0xd1, 0, true); // Expect: 00d1
  delay(TEST_DELAY);
  display.clear();
  display.showNumberHex(0xd1, 0, true, 2); // Expect: d1__
  delay(TEST_DELAY);

  // Test all the dots
  for (k = 0; k <= 4; k++) {
    display.showNumberDec(0, (0x80 >> k), true);
    delay(TEST_DELAY);
  }

  // Test Brightness Levels
  for (k = 0; k < 4; k++)
    data[k] = 0xff;
  for (k = 0; k < 7; k++) {
    display.setBrightness(k);
    display.setSegments(data);
    delay(TEST_DELAY);
  }

  // Test Display On/Off
  for (k = 0; k < 4; k++) {
    display.setBrightness(7, false);    // Turn off
    display.setSegments(data);
    delay(TEST_DELAY);
    display.setBrightness(7, true);     // Turn on
    display.setSegments(data);
    delay(TEST_DELAY);
  }

  // Test Horizontal Level Meter
  for (int count = 0; count < 3; count++) {
    for (int x = 0; x <= 100; x = x + 10) {
      display.showLevel(x, true);
      delay(20);
    }
    for (int x = 100; x > 0; x = x - 10) {
      display.showLevel(x, true);
      delay(20);
    }
  }
  // Test Vertical Level Meter
  for (int count = 0; count < 3; count++) {
    for (int x = 0; x <= 100; x = x + 10) {
      display.showLevel(x, false);
      delay(20);
    }
    for (int x = 100; x > 0; x = x - 10) {
      display.showLevel(x, false);
      delay(20);
    }
  }

  // Test Numbers and Strings  Using Positions
  display.clear();
  char degree[] = "\xB0";
  display.showString(degree, 1, 3);     // Position 3 (right) and 1 char length
  for (int x = -50; x < 150; x++) {
    display.showNumber(x, false, 3, 0); // Postion 0 (left) and 3 char length
    delay(10);
  }
  delay(TEST_DELAY);

  // Test String Display
  display.clear();
  display.showString("String Test 1234"); // Test literal string
  delay(TEST_DELAY);
  display.clear();
  char stringb[10];                 // Test dynamic string
  sprintf(stringb, "25%cC", '\xB0'); // Display 25 + degree symbol + C
  display.showString(stringb);
  delay(TEST_DELAY);
  display.clear();                  // Long string test
  display.showString("abcdefghijklmnopqrstuvwxyz.-=ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  delay(TEST_DELAY);

  // Animation Sequence Test - Run 3 times
  display.clear();
  for (int count = 0; count < 3; count++) {
    display.showAnimation(ANIMATION, FRAMES(ANIMATION), TIME_MS(10));
  }
  display.clear();
  delay(TEST_DELAY);

  // Done!
  display.clear();
  display.showString("The");
  delay(TEST_DELAY);
  display.showString("End");
  delay(TEST_DELAY);
  display.setSegments(SEG_DONE);
  delay(TEST_DELAY * 5);

}
