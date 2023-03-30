#include <vector>
#include <stdint.h>

#pragma once

class Sustain {
  private:
    typedef std::vector<unsigned long> scheduleV_t;
    enum ScheduleID
    {
      ACTIVATION,
      ON,
      DEACTIVATION,
      OFF
    };
    scheduleV_t schedule[4];
    int instances = 0;
    unsigned long timeSinceActivation = 0;
  
    void scheduleSustain(bool state);
    
  public:
    Sustain();
    void addToSchedule(uint8_t velocity);
    void processSchedule();
    void checkForErrors();
    void resetSchedule();
};

extern Sustain sustain;
