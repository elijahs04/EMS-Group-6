#include <Arduino.h>
#include "selfTest.h"

ST::ST(int pin) {
  pinMode(pin, OUTPUT);
  pin_ = pin;
}

void ST::beginST() {
  digitalWrite(pin_, HIGH);
}