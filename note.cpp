#include "note.h"

bool Note::getIsActive() { return this->isActive; }
unsigned long Note::getIsActiveSetAt() { return this->isActiveSetAt; }
int Note::getMidiId() { return this->midiId; }

void Note::setIsActive(bool isActive, unsigned long now) {
  this->isActive = isActive;
  this->isActiveSetAt = now;
}

int Note::calculateVelocity(int midiVelocity) {
  double slope = (this->maxVelocity - this->minVelocity) / 128.0;
  int mappedVelocity = this->minVelocity + round(slope * midiVelocity);
  return mappedVelocity;
}
