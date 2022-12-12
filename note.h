#include <string>
#include <cmath>
#include <stdint.h>
#include <Arduino.h>
#include <vector>
#include "settings.h"
#include "PCA9635.h"
#include "note_schedule.h"
#include "piano.h" // may not need this. depends on where piano is declared.

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
    void addToSchedule(int velocity, unsigned long delayedTime);
    void checkSchedule();
    int calculateVelocity(int midiVelocity);
    bool getIsActive();
    unsigned long getIsActiveSetAt();
    int getMidiId();
};
