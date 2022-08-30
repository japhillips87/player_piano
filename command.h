#include "note.h"
#include <stdint.h>

#pragma once

class Command {
  private:
    PCA9635 *board;
    int i2cIndex;
    int pwm;
    unsigned long runAt;

  public:
    Command(PCA9635 *board, int i2cIndex, int pwm, unsigned long runAt);
    PCA9635 *getBoard();
    int getI2cIndex();
    int getPwm();
    unsigned long getRunAt();
};
