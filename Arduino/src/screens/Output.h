#pragma once

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

class classOutput {
 private:
  SSD1306AsciiAvrI2c *pdisplay;

 public:
  classOutput();
  ~classOutput();
  void begin();
  void addText(int x, int y, const char *ptext);
  void drawRect(int x, int y, int width, int height);
  void clear();
  void flush();
};

extern classOutput Output;