#include <stdint.h>

#pragma once

class Command {
  private:
    int midiId;
    int pwm;

  public:
    Command(int midiId, int pwm)
      : midiId(midiId), pwm(pwm) {};
    int getPwm();
    int getMidiId();
};
