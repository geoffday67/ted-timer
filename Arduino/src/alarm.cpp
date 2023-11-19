#include "alarm.h"

PROGMEM Alarm::Note const Alarm::Tune[] = {
    {NOTE_C6, 3},
    {NOTE_A5, 2},
    {NOTE_G5, 1},
    {NOTE_A5, 3},
    {NOTE_G5, 2},
    {NOTE_E5, 1},
    {NOTE_G5, 3},
    {NOTE_E5, 2},
    {NOTE_D5, 1},
    {NOTE_E5, 3},

    {NOTE_E5, 1},
    {NOTE_A5, 1},
    {NOTE_C6, 1},
    {NOTE_D6, 3},
    {NOTE_C6, 2},
    {NOTE_A5, 1},
    {NOTE_C6, 3},
    {NOTE_A5, 2},
    {NOTE_G5, 1},
    {NOTE_A5, 3},
    {NOTE_G5, 2},
    {NOTE_E5, 1},
    {NOTE_G5, 3},

    {NOTE_F5, 1},
    {NOTE_GS5, 1},
    {NOTE_B5, 1},
    {NOTE_C6, 3},
    {NOTE_A5, 2},
    {NOTE_G5, 1},
    {NOTE_A5, 3},
    {NOTE_G5, 2},
    {NOTE_E5, 1},
    {NOTE_G5, 3},
    {NOTE_SILENT, 3},
};

Alarm::Alarm(byte pin) {
  pTone = new Tone;
  pTone->begin(pin);
  playing = 0;
}

Alarm::~Alarm() {
  delete pTone;
}

void Alarm::startNote() {
  Note note;
  
  memcpy_P(&note, Tune + currentNote, sizeof note);
  pTone->play(note.frequency, note.duration * unit);
}

void Alarm::start(int unit) {
  Serial.println(currentNote);
  this->unit = unit;
  currentNote = 0;
  playing = 1;
  startNote();
}

void Alarm::stop() {
  pTone->stop();
  playing = 0;
}

void Alarm::loop() {
  if (!playing) {
    return;
  }

  // Check if we're still playing, if not play the next note or loop.
  if (pTone->isPlaying()) {
    return;
  }

  if (++currentNote == (sizeof Tune) / (sizeof Tune[0])) {
    currentNote = 0;
  }

  startNote();
}