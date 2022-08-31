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
    // TODO: use the PCA9635 instance instead of the address
    //uint8_t boardAddress;
    PCA9635 *board;
    int boardIndex;
    int minVelocity;
    int maxVelocity;
    bool isActive;
    unsigned long isActiveSetAt;
    
  public:
    Note(int midiId, PCA9635 *board, int boardIndex, int minVelocity, int maxVelocity);

    void setIsActive(bool isActive, unsigned long now);
    PCA9635 *getBoard();
    int getBoardIndex();
    int calculateVelocity(int midiVelocity);
    bool getIsActive();
    unsigned long getIsActiveSetAt();
    int getMidiId();
};
