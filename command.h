#include <stdint.h>

#pragma once

class Command {
  private:
    int midiId;
    int pwm;
    unsigned long runAt;

  public:
    Command(int midiId, int pwm, unsigned long runAt)
      : midiId(midiId), pwm(pwm), runAt(runAt) {};
    int getPwm();
    int getMidiId();
    unsigned long getRunAt();
};
