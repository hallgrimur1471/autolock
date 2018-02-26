#include <bluefruit.h>
#include "car.h"

Car::Car(void) {
  _last_toggle_time = 0;
  _locked = true;
}

void Car::lock() { // (void) ?
  ledOn(LED_RED);
  _last_toggle_time = millis();
  _locked = true;
  Serial.println("lock");
}

void Car::unlock() {
  ledOff(LED_RED);
  _last_toggle_time = millis();
  _locked = false;
  Serial.println("unlock");
}

bool Car::isLocked() {
  return _locked;
}

int32_t Car::get_time_since_toggle() {
  return millis() - _last_toggle_time;
}