#include <vector>
#include <algorithm>
#include <stdint.h>
#include <Arduino.h>
#include "note.h"
#include "command.h"

#pragma once

using namespace std;

const int MIN_NOTE_ID = 21;
const int MAX_NOTE_ID = 108;
const int SUSTAIN_CONTROL_NUMBER = 64;
const int SUSTAIN_THRESHOLD = 64;
const int SUSTAIN_1_INDEX = 13;
const int SUSTAIN_2_INDEX = 14;

class Piano {
  // bool sustainIsActive;
  // unsigned long sustainIsActiveSetAt;

  public:
    Piano() {};
    void initialize();
    Note& find(int id);
    // bool getSustainIsActive();
    // unsigned long getSustainIsActiveSetAt();
    // void setSustainIsActive(bool isActive, unsigned long now);
    void scheduleNote(uint8_t midiId, uint8_t velocity);
    void handleNoteOn(uint8_t midiId, uint8_t velocity);
    void handleNoteOff(uint8_t midiId, uint8_t velocity);
    void handleControlChange(uint8_t number, uint8_t value);
    void addNote(Note &note);
    void addCommand(Command &command);
    vector<Note> notes;
    vector<Command> commands;
};

extern Piano piano;
