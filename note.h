#include <string>
#include <stdint.h>
#include <Arduino.h>
#include "settings.h"
#include "PCA9635.h"

#pragma once

using namespace std;

class Note {
  private:
    int midiId;
    int minVelocity;
    int maxVelocity;
    bool isActive;
    unsigned long isActiveSetAt;

  public:
    Note(int midiId, int minVelocity, int maxVelocity)
      : midiId(midiId), minVelocity(minVelocity), maxVelocity(maxVelocity), isActive(false), isActiveSetAt(millis()) {};

    void setIsActive(bool isActive, unsigned long now);
    int calculateVelocity(int midiVelocity);
    bool getIsActive();
    unsigned long getIsActiveSetAt();
    int getMidiId();
};
