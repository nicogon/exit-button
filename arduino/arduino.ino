#include <avr/wdt.h>
#include "DigiKeyboard.h"  //Keyboard Library
#define KEY_POWER 0x66  // Osx power key

unsigned long pullTime;
bool led;

void setup() {
  pinMode(0, INPUT); // P0 as Button Input
  pinMode(1, OUTPUT); // P1 as Output
  digitalWrite(0, HIGH); // Enable Internal Pull-up
  digitalWrite(1, HIGH); // Turn onboard LED (P1) on
  // Prevent reset on while{}
  wdt_disable();
}

void loop() {
  DigiKeyboard.sendKeyStroke(0);
  int sensorValue = digitalRead(0);
  if (sensorValue == 0) {
    pullTime = millis();
    // Key bounce intro
    DigiKeyboard.delay(50);
    while (!digitalRead(0)) {}
    if (millis() - pullTime > 300) {
      // More than 300ms, write computer lock command
      DigiKeyboard.sendKeyStroke(KEY_POWER, MOD_ALT_LEFT | MOD_GUI_LEFT);
    } else {
      // Les than 300ms, write command+w (close tab)
      DigiKeyboard.sendKeyStroke(KEY_W, MOD_GUI_LEFT);
    }
    led = !led;
    // Switch led to detect attiny is not dead
    digitalWrite(1, led);
    // Key bounce exit
    DigiKeyboard.delay(200);
  }
}
