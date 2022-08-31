#include <vector>
#include <stdint.h>
#include "command.h"
#include "settings.h"

#pragma once

class Schedule {
  vector<Command> commands;
  public:
    Schedule();
    void tryToScheduleNoteOn(uint8_t noteId, uint8_t velocity);
    void tryToScheduleNoteOff(uint8_t noteId, uint8_t velocity);
    void tryToScheduleSustain(uint8_t number, uint8_t value);
    void scheduleSustainOn(unsigned long delayedTime = millis());
    void scheduleNoteOn(Note &note, int velocity, unsigned long delayedTime = millis());
    void execute();
};

extern Schedule schedule;
