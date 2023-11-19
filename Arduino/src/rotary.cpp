#include "rotary.h"

Rotary::Rotary(byte clk, byte dt) {
  this->clk = clk;
  this->dt = dt;
  prevNextCode = 0;
  store = 0;
}

void Rotary::begin() {
  pinMode(clk, INPUT_PULLUP);
  pinMode(dt, INPUT_PULLUP);
}

int Rotary::getDirection() {
  static uint16_t rot_enc_table = 0x6996;

  prevNextCode <<= 2;
  if (digitalRead(dt)) prevNextCode |= 0x02;
  if (digitalRead(clk)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

  // If valid then store as 16 bit data.
  if (rot_enc_table & (0x8000 >> prevNextCode)) {
    store <<= 4;
    store |= prevNextCode;
    if ((store & 0xff) == 0x2b) return -1;
    if ((store & 0xff) == 0x17) return 1;
  }
  return 0;
}