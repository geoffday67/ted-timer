#pragma once

#include <Time.h>

#include <queue>
#include <vector>

#include "Event.h"
#include "EventReceiver.h"

#define EVENT_ENCODER 1
#define EVENT_MENU 2
#define EVENT_SWITCH 3
#define EVENT_BUTTON 4
#define EVENT_SECOND 5

class EncoderEvent : public Event {
 public:
  EncoderEvent(int step);
  int step;
};

class MenuEvent : public Event {
 public:
  MenuEvent(int choice);
  int choice;
};

class SwitchEvent : public Event {
 public:
  SwitchEvent(bool pressed);
  bool pressed;
};

class ButtonEvent : public Event {
 public:
  ButtonEvent(bool pressed) : Event(EVENT_BUTTON), pressed(pressed) {}
  bool pressed;
};

class SecondEvent : public Event {
 public:
  SecondEvent() : Event(EVENT_SECOND) {}
};

/*****************************************************************************/
class RegisteredReceiver {
 public:
  RegisteredReceiver(EventReceiver *per, int type);
  EventReceiver *pEventReceiver;
  int type;
  bool fired;
};

/*****************************************************************************/
class classEventManager {
 public:
  void addListener(int type, EventReceiver *per);
  void removeListener(EventReceiver *per);
  void queueEvent(Event *pevent);
  void processEvents();
  void emptyQueue();

 private:
  std::queue<Event *> queueEvents;
  std::vector<RegisteredReceiver> listReceivers;
};

extern classEventManager EventManager;
