#include "note.h"

Note::Note(int midiId, PCA9635 *board, int boardIndex, int minVelocity, int maxVelocity, bool isActive) {
  this->midiId = midiId;
  this->board = board;
  this->boardIndex = boardIndex;
  this->minVelocity = minVelocity;
  this->maxVelocity = maxVelocity;
  this->setIsActive(isActive, millis());
}

PCA9635 *Note::getBoard() { return this->board; }
int Note::getBoardIndex() { return this->boardIndex; }
bool Note::getIsActive() { return this->isActive; }
unsigned long Note::getIsActiveSetAt() { return this->isActiveSetAt; }
int Note::getMidiId() { return this->midiId; }

void Note::setIsActive(bool isActive, unsigned long now) { 
  this->isActive = isActive;
  this->isActiveSetAt = now;
}

int Note::calculateVelocity(int midiVelocity) {
  return this->minVelocity + ((this->maxVelocity - this->minVelocity) / (MAX_MIDI_VELOCITY - MIN_MIDI_VELOCITY)) * (midiVelocity - this->minVelocity);
}
