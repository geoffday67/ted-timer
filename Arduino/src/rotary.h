#pragma once

#include <Arduino.h>

class Rotary {
 private:
  byte clk, dt;
  uint16_t prevNextCode;
  uint16_t store;

 public:
  Rotary(byte clk, byte dt);
  void begin();
  int getDirection();
};