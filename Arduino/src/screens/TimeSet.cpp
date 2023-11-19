#include "TimeSet.h"

#include <Arduino.h>

#include "../alarm.h"
#include "../pins.h"

extern Alarm *pAlarm;

void TimeSet::activate() {
  EventManager.addListener(EVENT_ENCODER, this);
  EventManager.addListener(EVENT_SWITCH, this);
  EventManager.addListener(EVENT_BUTTON, this);
  EventManager.addListener(EVENT_SECOND, this);

  state = Setting;
  type = Minutes;
  minutes = 0;
  seconds = 0;
  running = 0;
  showCurrent();
  showIndicator();
}

void TimeSet::deactivate() {
  EventManager.removeListener(this);
  delete this;
}

bool TimeSet::onEvent(Event *pevent) {
  switch (pevent->type) {
    case EVENT_ENCODER:
      handleEncoderEvent((EncoderEvent *)pevent);
      break;
    case EVENT_SWITCH:
      handleSwitchEvent((SwitchEvent *)pevent);
      break;
    case EVENT_BUTTON:
      handleButtonEvent((ButtonEvent *)pevent);
      break;
    case EVENT_SECOND:
      handleSecondEvent((SecondEvent *)pevent);
      break;
  }
  return true;
}

void TimeSet::handleEncoderEvent(EncoderEvent *pevent) {
  int previous;

  switch (state) {
    case Setting:
      switch (type) {
        case Minutes:
          previous = minutes;
          minutes += pevent->step;
          if (minutes > 240) minutes = 240;
          if (minutes < 0) minutes = 0;
          if (previous != minutes) {
            showCurrent();
          }
          break;

        case Seconds:
          previous = seconds;
          seconds += pevent->step;
          if (seconds < 0) seconds = 0;
          if (seconds > 59) seconds = 59;
          if (previous != seconds) {
            showCurrent();
          }
          break;
      }
      break;

    default:
      break;
  }
}

void TimeSet::handleSwitchEvent(SwitchEvent *pevent) {
  if (!pevent->pressed) return;

  switch (state) {
    case Setting:
      if (type == Minutes) {
        type = Seconds;
      } else {
        type = Minutes;
      }
      showIndicator();
      break;

    default:
      break;
  }
}

void TimeSet::handleButtonEvent(ButtonEvent *pevent) {
  if (!pevent->pressed) return;

  switch (state) {
    case Setting:
      if (minutes > 0 || seconds > 0) {
        hideIndicator();
        state = Running;
      }
      break;

    case Running:
      showIndicator();
      state = Setting;
      break;

    case Alarming:
      digitalWrite(LIGHT_PIN, LOW);
      pAlarm->stop();
      state = Setting;
      showIndicator();
      break;

    default:
      break;
  }
}

void TimeSet::handleSecondEvent(SecondEvent *pevent) {
  switch (state) {
    case Running:
      if (--seconds < 0) {
        minutes--;
        seconds = 59;
      }
      showCurrent();

      if (minutes == 0 && seconds == 0) {
        pAlarm->start(250);
        digitalWrite(LIGHT_PIN, HIGH);
        state = Alarming;
      }

      break;

    default:
      break;
  }
}

void TimeSet::showCurrent() {
  char buffer[16];

  sprintf(buffer, "%02d:%02d", minutes, seconds);
  Output.addText(30, 0, buffer);
}

void TimeSet::showIndicator() {
  switch (type) {
    case Minutes:
      Output.addText(6, 0, ">");
      Output.addText(110, 0, " ");
      break;
    case Seconds:
      Output.addText(6, 0, " ");
      Output.addText(110, 0, "<");
      break;
  }
}

void TimeSet::hideIndicator() {
  Output.addText(6, 0, " ");
  Output.addText(110, 0, " ");
}
