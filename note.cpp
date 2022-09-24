#include "note.h"

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
