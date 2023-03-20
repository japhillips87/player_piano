#include "piano.h"

void Piano::addNote(Note &note) { notes.push_back(note); }
void Piano::addCommand(Command command) { commands.push_back(command); }


Note& Piano::find(int id) {
  auto note_it = find_if(begin(notes), end(notes), [&id](Note note) { return note.getMidiId() == id; });
  return *note_it;
}

void Piano::initialize() {
  for (int id = MIN_NOTE_ID; id <= MAX_NOTE_ID; id++) {
    Note &note = *new Note(id);
    note.resetSchedule();
    addNote(note);
  }
}

void Piano::scheduleNote(uint8_t midiId, uint8_t velocity) {
  Note &note = find(midiId);
  note.addToSchedule(velocity);
}

void Piano::scheduleSustain(uint8_t channel, uint8_t number, uint8_t value) {
  if(number == SUSTAIN_CONTROL_NUMBER && channel == 1) {
    sustain.addToSchedule(value);
  }
}
