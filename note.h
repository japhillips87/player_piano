#include <string>
#include <cmath>
#include <stdint.h>
#include <Arduino.h>
#include <vector>
#include "settings.h"
#include "PCA9635.h"
#include "note_schedule.h"

#pragma once

using namespace std;

class Note {
  private:
    int midiId;
    int minVelocity;
    int maxVelocity;
    bool isActive;
    unsigned long isActiveSetAt;
    vector<NoteSchedule> schedule;

  public:
    Note(int midiId, int minVelocity, int maxVelocity)
      : midiId(midiId), minVelocity(minVelocity), maxVelocity(maxVelocity), isActive(false), isActiveSetAt(millis()) {};

    void setIsActive(bool isActive, unsigned long now);
    void addToSchedule(string type, int velocity, unsigned long delayedTime);
    void processSchedule();
    int calculateVelocity(int midiVelocity);
    int getMidiId();
    void commandNote(NoteSchedule& scheduledNote);
    void commandNoteOn(NoteSchedule& scheduledNote);
    void commandFastNoteOn();
    void commandNoteOff(NoteSchedule& scheduledNote);
    // TODO: these should be able to be removed. the note's state should be able to be
    // obtained from the schedule
    bool getIsActive();
    unsigned long getIsActiveSetAt();
};
