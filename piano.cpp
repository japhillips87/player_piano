#include "piano.h"

Piano::Piano() {
  PCA9635 board1(0x40);
  
  this->setSustainIsActive(false, millis());
  notes.push_back(Note(21, &board1, 0, 0, 255, false));
  this->setSustainBoard(&board1);
}

bool Piano::getSustainIsActive() { return this->sustainIsActive; }
unsigned long Piano::getSustainIsActiveSetAt() { return this->sustainIsActiveSetAt; }
PCA9635 *Piano::getSustainBoard() { return this->sustainBoard; }
void Piano::setSustainBoard(PCA9635 *board) { this->sustainBoard = board; }
void Piano::setSustainIsActive(bool isActive, unsigned long now) { 
  this->sustainIsActive = isActive;
  this->sustainIsActiveSetAt = now;
} 

Note Piano::find(int id) {
  auto note_it = find_if(begin(notes), end(notes), [&id](Note note) { return note.getMidiId() == id; });
  return *note_it;
}
