#include "note.h"
#include "piano.h"

#pragma once

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

void Note::addToSchedule(string type, int velocity, unsigned long delayedTime) {
  schedule.push_back(*new NoteSchedule(type, velocity, delayedTime));
}

void Note::processSchedule() {
  if (schedule.size() == 0) { // there is no schedule for this note
    return;
  } else if (schedule.size() == 1 && schedule.front().getRunAt() <= millis()) { // there is only one scheduled event and the delay has been reached
    commandNote(schedule.front());
    schedule.erase(schedule.begin());
  } else { // there is either more than one event or the delay hasn't been reached
    unsigned long now = millis();
    for (auto it = schedule.begin(); it != schedule.end(); it++) {
      /*
      * if (note has more than one thing scheduled) {
      *   if there is enough time to schedule normally { do it }
      *   else if there is not enough time { adjust the schedule }
      * } else (there is only one thing scheduled and delay hasn't been reached) { no-op }
      *
      *
      */
      // compare runAt to the current time.
      // compare runAt of the first command to the next scheduled command if it exists
      // be sure to remove things from schedule as they are added to commands
      // if (now <= it->getRunAt())
    }
  }
  //piano.addCommand(*new Command(48, 255, millis()));
}

void Note::commandNote(NoteSchedule &scheduledNote) {
  if (scheduledNote.getType() == "ON") {
    commandNoteOn(scheduledNote);
  } else {
    commandNoteOff(scheduledNote);
  }
}

void Note::commandNoteOn(NoteSchedule &scheduledNote) {
  unsigned long delayedTime = scheduledNote.getRunAt();

  // TODO: set the isActive flag with correct time
  piano.addCommand(*new Command(this->getMidiId(), ON_PWM, delayedTime));
  piano.addCommand(*new Command(this->getMidiId(), this->calculateVelocity(scheduledNote.getVelocity()), delayedTime + STARTUP_DURATION));
  piano.addCommand(*new Command(this->getMidiId(), NOTE_HOLD_PWM, delayedTime + TOTAL_ON_DURATION));
}

void Note::commandNoteOff(NoteSchedule &scheduledNote) {
  piano.addCommand(*new Command(this->getMidiId(), OFF_PWM, scheduledNote.getRunAt()));
}
