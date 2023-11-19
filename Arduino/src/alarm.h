#pragma once

#include <Arduino.h>
#include <Tone.h>

class Alarm {
 private:
  typedef struct {
    int frequency;
    int duration;
  } Note;

  static const Note Tune[];

  Tone *pTone;
  byte currentNote, playing;
  int unit;

  void startNote();

 public:
  Alarm(byte pin);
  ~Alarm();
  void start(int unit);
  void stop();
  void loop();
};