#include <vector>
#include <algorithm>
#include <stdint.h>
#include <Arduino.h>
#include "note.h"
#include "command.h"
#include "sustain.h"

#pragma once

using namespace std;

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
    void scheduleSustain(uint8_t channel, uint8_t number, uint8_t value);
    void addNote(Note &note);
    void addCommand(Command command);
    vector<Note> notes;
    vector<Command> commands;
};

extern Piano piano;
