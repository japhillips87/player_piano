#include "schedule.h"
#include "piano.h"
#include <Arduino.h>

Schedule::Schedule() {}

void Schedule::tryToScheduleNoteOn(uint8_t noteId, uint8_t velocity) {
  if (noteId >= MIN_NOTE_ID && noteId <= MAX_NOTE_ID) {
    Note &note = piano.find(noteId);
    bool isActive = note.getIsActive();
    unsigned long isActiveSetAt = note.getIsActiveSetAt();

    // Only schedule a note if it is not currently active. If the note has been
    // recently deactivated but hasn't had time to reset, delay the activation just a bit.
    if (!isActive) {
      if (isActiveSetAt + DEACTIVATE_TIME_REQUIRED <= millis()) {
        scheduleNoteOn(note, velocity);
      } else {
        unsigned long delayedTime = isActiveSetAt + DEACTIVATE_TIME_REQUIRED;
        scheduleNoteOn(note, velocity, delayedTime);
      }
    }
  }
}

void Schedule::scheduleNoteOn(Note &note, int velocity, unsigned long delayedTime /*= millis()*/) {
  int midiId = note.getMidiId();
  int mappedVelocity = note.calculateVelocity(velocity);

  note.setIsActive(true, delayedTime);
  commands.push_back(Command(midiId, ON_PWM, delayedTime));
  commands.push_back(Command(midiId, mappedVelocity, delayedTime + STARTUP_DURATION));
  commands.push_back(Command(midiId, NOTE_HOLD_PWM, delayedTime + TOTAL_ON_DURATION));
}

void Schedule::scheduleNoteOff(Note &note, unsigned long delayedTime /* = millis()*/) {
  note.setIsActive(false, delayedTime);
  commands.push_back(Command(note.getMidiId(), OFF_PWM, delayedTime));
}

void Schedule::scheduleNoteOnForDuration(Note &note, int velocity, unsigned long delayedTime, int duration) {
  scheduleNoteOn(note, velocity, delayedTime);
  scheduleNoteOff(note, delayedTime + duration);
}

void Schedule::tryToScheduleNoteOff(uint8_t noteId, uint8_t velocity) {
  if (noteId >= MIN_NOTE_ID && noteId <= MAX_NOTE_ID) {
    Note &note = piano.find(noteId);
    bool isActive = note.getIsActive();
    unsigned long isActiveSetAt = note.getIsActiveSetAt();
    unsigned long now = millis();

    // Only schedule a note off if the note is currently active. If the note has been
    // recently activated but hasn't had time to play, delay the activation for just a bit.
    if (isActive) {
      if (isActiveSetAt + TOTAL_ON_DURATION <= now) {
        scheduleNoteOff(note);
      } else {
        unsigned long delayedTime = isActiveSetAt + TOTAL_ON_DURATION;
        scheduleNoteOff(note, delayedTime);
      }
    }
  }
}

void Schedule::tryToScheduleSustain(uint8_t number, uint8_t value) {
  bool isActive = piano.getSustainIsActive();
  unsigned long isActiveSetAt = piano.getSustainIsActiveSetAt();
  unsigned long now = millis();
  if (number == SUSTAIN_CONTROL_NUMBER) {
    if (value >= SUSTAIN_THRESHOLD && !isActive) {
      if (isActiveSetAt + SUSTAIN_DEACTIVATE_TIME_REQUIRED <= millis()) {
        // scheduleSustainOn();
      } else {
        unsigned long delayedTime = isActiveSetAt + SUSTAIN_DEACTIVATE_TIME_REQUIRED;
        // scheduleSustainOn(delayedTime);
      }
    } else if (value <= SUSTAIN_THRESHOLD && isActive) {
      if (isActiveSetAt + SUSTAIN_TOTAL_ON_DURATION <= now) {
        // scheduleSustainOff();
      } else {
        unsigned long delayedTime = isActiveSetAt + SUSTAIN_TOTAL_ON_DURATION;
        // scheduleSustainOff(delayedTime);
      }
    }
  } else if (number == 123) {
    // TODO: this is running a bunch of times. debug it
    //allOff();
  }
}

// void Schedule::scheduleSustainOn(unsigned long delayedTime /*=millis()*/) {
//   PCA9635 &board = piano.getSustainBoard();

//   piano.setSustainIsActive(true, delayedTime);
//   commands.push_back(Command(board, SUSTAIN_1_INDEX, ON_PWM, delayedTime));
//   commands.push_back(Command(board, SUSTAIN_1_INDEX, SUSTAIN_VELOCITY, delayedTime + SUSTAIN_STARTUP_DURATION));
//   commands.push_back(Command(board, SUSTAIN_1_INDEX, SUSTAIN_HOLD_PWM, delayedTime + SUSTAIN_TOTAL_ON_DURATION));

//   commands.push_back(Command(board, SUSTAIN_2_INDEX, ON_PWM, delayedTime));
//   commands.push_back(Command(board, SUSTAIN_2_INDEX, SUSTAIN_VELOCITY, delayedTime + SUSTAIN_STARTUP_DURATION));
//   commands.push_back(Command(board, SUSTAIN_2_INDEX, SUSTAIN_HOLD_PWM, delayedTime + SUSTAIN_TOTAL_ON_DURATION));
// }

// void Schedule::scheduleSustainOff(unsigned long delayedTime /*=millis()*/) {
//   piano.setSustainIsActive(false, delayedTime);
//   commands.push_back(Command(piano.getSustainBoard(), SUSTAIN_1_INDEX, OFF_PWM, delayedTime));
//   commands.push_back(Command(piano.getSustainBoard(), SUSTAIN_2_INDEX, OFF_PWM, delayedTime));
// }

void Schedule::connected() {
  unsigned long delay = millis();
  int noteIds[] = { 74, 81, 86 };

  for (int noteId: noteIds) {
    Note &note = piano.find(noteId);
    scheduleNoteOnForDuration(note, MIDI_HALF_VELOCITY, delay, BLUETOOTH_SOUND_DURATION);
    delay += BLUETOOTH_SOUND_DELAY;
  }
}

void Schedule::disconnected() {
  unsigned long delay = millis();
  int noteIds[] = { 86, 81, 74 };

  for (int noteId: noteIds) {
    Note &note = piano.find(noteId);
    scheduleNoteOnForDuration(note, MIDI_HALF_VELOCITY, delay, BLUETOOTH_SOUND_DURATION);
    delay += BLUETOOTH_SOUND_DELAY;
  }
}

void Schedule::poweredOn() {
  unsigned long delay = millis();
  int noteIds[] = { 34, 46, 50, 53, 58, 62, 65, 70, 74, 77, 82 };

  for (int noteId: noteIds) {
    Note &note = piano.find(noteId);
    scheduleNoteOnForDuration(note, MIDI_HALF_VELOCITY, delay, POWER_SOUND_DURATION_ONE);
    scheduleNoteOnForDuration(note, MIDI_HALF_VELOCITY, delay + POWER_SOUND_DELAY, POWER_SOUND_DURATION_TWO);
  }
}

void Schedule::allOff() {
  for (Note &note: piano.notes) {
    scheduleNoteOff(note);
  }
  scheduleSustainOff();
}

void Schedule::execute() {
  for (auto it = commands.begin(); it != commands.end(); it++) {
    if (it->getRunAt() <= millis()) {
      // TODO: make this talk to the electronics instead
      Serial.print("RUNNING COMMAND: ");
      Serial.print("Midi Id: ");
      Serial.print(it->getMidiId());
      Serial.print(", PWM: ");
      Serial.println(it->getPwm());
      commands.erase(it--);
    }
  }
}
