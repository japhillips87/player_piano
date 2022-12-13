#include "piano.h"

bool Piano::getSustainIsActive() { return this->sustainIsActive; }
unsigned long Piano::getSustainIsActiveSetAt() { return this->sustainIsActiveSetAt; }
// PCA9635 &Piano::getSustainBoard() { return this->sustainBoard; }
// void Piano::setSustainBoard(PCA9635 &board) { this->sustainBoard = board; }
void Piano::addNote(Note &note) { notes.push_back(note); }
void Piano::addCommand(Command &command) { commands.push_back(command); }
void Piano::setSustainIsActive(bool isActive, unsigned long now) {
  this->sustainIsActive = isActive;
  this->sustainIsActiveSetAt = now;
}

Note& Piano::find(int id) {
  auto note_it = find_if(begin(notes), end(notes), [&id](Note note) { return note.getMidiId() == id; });
  return *note_it;
}

void Piano::initialize() {
  for (int i = 21; i <= 108; i++) {
    addNote(*new Note(i, NOTE_MIN_PWM, NOTE_MAX_PWM));
  }

//  this->setSustainIsActive(false, millis());
//  this->setSustainBoard(&board7);
}

void Piano::handleNoteOn(uint8_t midiId, uint8_t velocity) {
  Note &note = find(midiId);
  note.addToSchedule("ON", velocity, millis() + DEFAULT_DELAY);
}

void Piano::handleNoteOff(uint8_t midiId, uint8_t velocity) {
  Note &note = find(midiId);
  note.addToSchedule("OFF", velocity, millis() + DEFAULT_DELAY);
}
