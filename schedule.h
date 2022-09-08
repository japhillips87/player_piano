#include <vector>
#include <stdint.h>
#include "command.h"
#include "settings.h"

#pragma once

class Schedule {
  public:
    Schedule();
    void tryToScheduleNoteOn(uint8_t noteId, uint8_t velocity);
    void tryToScheduleNoteOff(uint8_t noteId, uint8_t velocity);
    void tryToScheduleSustain(uint8_t number, uint8_t value);
    void scheduleSustainOn(unsigned long delayedTime = millis());
    void scheduleSustainOff(unsigned long delayedTime = millis());
    void scheduleNoteOn(Note &note, int velocity, unsigned long delayedTime = millis());
    void scheduleNoteOff(Note &note, unsigned long delayedTime = millis());
    void scheduleNoteOnForDuration(Note &note, int velocity, unsigned long delayedTime, int duration);
    void connected();
    void disconnected();
    void allOff();
    void poweredOn();
    void execute();
    vector<Command> commands;
};

extern Schedule schedule;
