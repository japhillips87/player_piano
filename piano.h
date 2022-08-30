#include <vector>
#include <algorithm>
#include <stdint.h>
#include <Arduino.h>
#include "PCA9635.h"
#include "note.h"

#pragma once

using namespace std;

const int MIN_NOTE_ID = 21;
const int MAX_NOTE_ID = 108;
const int SUSTAIN_CONTROL_NUMBER = 64;
const int SUSTAIN_INDEX = 15;

class Piano {
  vector<Note> notes;
  bool sustainIsActive;
  unsigned long sustainIsActiveSetAt;
  PCA9635 *sustainBoard;
  
  public:
    Piano();
    Note find(int id);
    bool getSustainIsActive();
    unsigned long getSustainIsActiveSetAt();
    void setSustainIsActive(bool isActive, unsigned long now);
    void setSustainBoard(PCA9635 *board);
    PCA9635 *getSustainBoard();
};

extern Piano piano;
