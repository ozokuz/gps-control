#include "Pin.hpp"

Pin::Pin(uint8_t pin) : _pin(pin) {
  digitalWrite(_pin, LOW);
}

void Pin::Toggle() {
  _state = !_state;
  digitalWrite(_pin, _state ? HIGH : LOW);
}

void Pin::SetLow() {
  _state = false;
  digitalWrite(_pin, LOW);
}

void Pin::SetHigh() {
  _state = true;
  digitalWrite(_pin, HIGH);
}