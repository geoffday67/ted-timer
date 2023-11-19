#pragma once

#include "EventManager.h"
#include "EventReceiver.h"
#include "Screen.h"

enum TimeSetType {
  Minutes, Seconds
};

enum State {
  Setting, Running, Alarming
};

class TimeSet : public Screen, EventReceiver {
 private:
  State state;
  TimeSetType type;
  int minutes, seconds;
  byte running, alarming;
  void showCurrent();
  void showIndicator();
  void hideIndicator();
  void handleEncoderEvent(EncoderEvent*);
  void handleSwitchEvent(SwitchEvent*);
  void handleButtonEvent(ButtonEvent*);
  void handleSecondEvent(SecondEvent*);

 public:
  virtual void activate();
  virtual void deactivate();
  virtual bool onEvent(Event* pevent);
};