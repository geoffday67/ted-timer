#include <Arduino.h>
#include <Debouncer.h>

#include "LowPower.h"
#include "alarm.h"
#include "pins.h"
#include "rotary.h"
#include "screens/EventManager.h"
#include "screens/Output.h"
#include "screens/TimeSet.h"

#define LOW_POWER_LEVEL 3400

Rotary encoder(ENCODER_CLK, ENCODER_DT);
Alarm *pAlarm;
unsigned long previousMillis;

int getEncoder() {
  return digitalRead(ENCODER_BUTTON) == 0;
}

void encoderChanged(int value) {
  EventManager.queueEvent(new SwitchEvent(value));
}

Debouncer encoderDebouncer(getEncoder, encoderChanged, 100);

int getButton() {
  return digitalRead(BUTTON_PIN) == 0;
}

void buttonChanged(int value) {
  EventManager.queueEvent(new ButtonEvent(value));
}

Debouncer buttonDebouncer(getButton, buttonChanged, 100);

int readVcc() {
  int result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);             // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);  // Convert
  while (bit_is_set(ADCSRA, ADSC))
    ;
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result;  // Back-calculate AVcc in mV
  return result;
}

void setup(void) {
  Serial.begin(115200);

  pAlarm = new Alarm(BUZZER_PIN);
  Serial.println(F("Alarm initialised"));

  Output.begin();
  Serial.println(F("Output initialised"));

  digitalWrite(LOW_POWER_PIN, LOW);
  pinMode(LOW_POWER_PIN, OUTPUT);
  Serial.println(F("Low battery initialised"));

  pinMode(ENCODER_BUTTON, INPUT);
  encoder.begin();
  Serial.println(F("Encoder initialised"));

  digitalWrite(LIGHT_PIN, LOW);
  pinMode(LIGHT_PIN, OUTPUT);
  Serial.println(F("Light initialised"));

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println(F("Button initialised"));

  previousMillis = 0;

  Screen *pscreen = new TimeSet();
  pscreen->activate();
}

void checkPower() {
  int level = readVcc();

  if (level < LOW_POWER_LEVEL) {
    digitalWrite(LOW_POWER_PIN, HIGH);
  } else {
    digitalWrite(LOW_POWER_PIN, LOW);
  }
}

void loop(void) {
  int direction;
  unsigned long now;

  now = millis();
  if (now - previousMillis >= 1000) {
    checkPower();
    EventManager.queueEvent(new SecondEvent());
    previousMillis = now;
  }

  buttonDebouncer.loop();
  encoderDebouncer.loop();
  pAlarm->loop();

  if (direction = encoder.getDirection()) {
    EventManager.queueEvent(new EncoderEvent(direction));
  }

  EventManager.processEvents();
}