#pragma once

#include <Arduino.h>

class Pin {
 public:
  Pin(uint8_t pin);
  void Toggle();

 private:
  uint8_t _pin;
  bool _state = false;
};