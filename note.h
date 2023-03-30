#include <string>
#include <cmath>
#include <stdint.h>
#include <Arduino.h>
#include <vector>
#include "settings.h"

#pragma once

using namespace std;

class Note {
  private:
    typedef std::vector<unsigned long> scheduleV_t;
    enum ScheduleID
    {
      STARTUP,
      ACTIVATION,
      VELOCITY,
      ON,
      DEACTIVATION,
      OFF
    };
    scheduleV_t schedule[6];
    int midiId;
    int instances = 0;
    int startupMs = MAX_STARTUP_MS;
    int deactivateMs = MAX_DEACTIVATE_MS;
    unsigned long timeSinceActivation = 0;

  public:
    Note(int midiId)
      : midiId(midiId) {};

    void setIsActive(bool isActive, unsigned long now);
    void addToSchedule(int velocity);
    void checkForErrors();
    void resetSchedule();
    void processSchedule();
    int calculateVelocity(int midiVelocity);
    int getMidiId();
};
