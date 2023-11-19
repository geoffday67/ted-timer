#include "Output.h"

classOutput Output;

classOutput::classOutput() {
  pdisplay = new SSD1306AsciiAvrI2c();
}

classOutput::~classOutput() {
  delete pdisplay;
}

void classOutput::begin() {
  pdisplay->begin(&Adafruit128x32, 0x3C);
  pdisplay->setI2cClock(400000);
  pdisplay->setFont(X11fixed7x14);
  pdisplay->set2X();
  pdisplay->clear();
}

void classOutput::flush() {
}

void classOutput::addText(int x, int y, const char *ptext) {
  pdisplay->setCursor(x, y);
  pdisplay->print(ptext);
}

void classOutput::drawRect(int x, int y, int width, int height) {
}

void classOutput::clear() {
  pdisplay->clear();
}