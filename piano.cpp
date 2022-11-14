#include "piano.h"

bool Piano::getSustainIsActive() { return this->sustainIsActive; }
unsigned long Piano::getSustainIsActiveSetAt() { return this->sustainIsActiveSetAt; }
// PCA9635 &Piano::getSustainBoard() { return this->sustainBoard; }
// void Piano::setSustainBoard(PCA9635 &board) { this->sustainBoard = board; }
void Piano::addNote(Note &note) { notes.push_back(note); }
void Piano::setSustainIsActive(bool isActive, unsigned long now) {
  this->sustainIsActive = isActive;
  this->sustainIsActiveSetAt = now;
}

Note& Piano::find(int id) {
  auto note_it = find_if(begin(notes), end(notes), [&id](Note note) { return note.getMidiId() == id; });
  return *note_it;
}

void Piano::initialize() {

//  PCA9635 board2(0x41);
//  PCA9635 board3(0x42);
//  PCA9635 board4(0x43);
//  PCA9635 board5(0x44);
//  PCA9635 board6(0x45);
//  PCA9635 board7(0x46);

//  board2.begin(sda_pin, scl_pin);
//  board3.begin(sda_pin, scl_pin);
//  board4.begin(sda_pin, scl_pin);
//  board5.begin(sda_pin, scl_pin);
//  board6.begin(sda_pin, scl_pin);
//  board7.begin(sda_pin, scl_pin);

  this->addNote(*new Note(21, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(22, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(23, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(24, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(25, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(26, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(27, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(28, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(29, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(30, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(31, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(32, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(33, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(34, NOTE_MIN_PWM, NOTE_MAX_PWM));
  this->addNote(*new Note(35, NOTE_MIN_PWM, NOTE_MAX_PWM));
//  this->addNote(*new Note(36, &board2, 0, 0, 255));
//  this->addNote(*new Note(37, &board2, 1, 0, 255));
//  this->addNote(*new Note(38, &board2, 2, 0, 255));
//  this->addNote(*new Note(39, &board2, 3, 0, 255));
//  this->addNote(*new Note(40, &board2, 4, 0, 255));
//  this->addNote(*new Note(41, &board2, 5, 0, 255));
//  this->addNote(*new Note(42, &board2, 6, 0, 255));
//  this->addNote(*new Note(43, &board2, 7, 0, 255));
//  this->addNote(*new Note(44, &board2, 8, 0, 255));
//  this->addNote(*new Note(45, &board2, 9, 0, 255));
//  this->addNote(*new Note(46, &board2, 10, 0, 255));
//  this->addNote(*new Note(47, &board2, 11, 0, 255));
//  this->addNote(*new Note(48, &board3, 0, 0, 255));
//  this->addNote(*new Note(49, &board3, 1, 0, 255));
//  this->addNote(*new Note(50, &board3, 2, 0, 255));
//  this->addNote(*new Note(51, &board3, 3, 0, 255));
//  this->addNote(*new Note(52, &board3, 4, 0, 255));
//  this->addNote(*new Note(53, &board3, 5, 0, 255));
//  this->addNote(*new Note(54, &board3, 6, 0, 255));
//  this->addNote(*new Note(55, &board3, 7, 0, 255));
//  this->addNote(*new Note(56, &board3, 8, 0, 255));
//  this->addNote(*new Note(57, &board3, 9, 0, 255));
//  this->addNote(*new Note(58, &board3, 10, 0, 255));
//  this->addNote(*new Note(59, &board3, 11, 0, 255));
//  this->addNote(*new Note(60, &board4, 0, 0, 255));
//  this->addNote(*new Note(61, &board4, 1, 0, 255));
//  this->addNote(*new Note(62, &board4, 2, 0, 255));
//  this->addNote(*new Note(63, &board4, 3, 0, 255));
//  this->addNote(*new Note(64, &board4, 4, 0, 255));
//  this->addNote(*new Note(65, &board4, 5, 0, 255));
//  this->addNote(*new Note(66, &board4, 6, 0, 255));
//  this->addNote(*new Note(67, &board4, 7, 0, 255));
//  this->addNote(*new Note(68, &board4, 8, 0, 255));
//  this->addNote(*new Note(69, &board4, 9, 0, 255));
//  this->addNote(*new Note(70, &board4, 10, 0, 255));
//  this->addNote(*new Note(71, &board4, 11, 0, 255));
//  this->addNote(*new Note(72, &board5, 0, 0, 255));
//  this->addNote(*new Note(73, &board5, 1, 0, 255));
//  this->addNote(*new Note(74, &board5, 2, 0, 255));
//  this->addNote(*new Note(75, &board5, 3, 0, 255));
//  this->addNote(*new Note(76, &board5, 4, 0, 255));
//  this->addNote(*new Note(77, &board5, 5, 0, 255));
//  this->addNote(*new Note(78, &board5, 6, 0, 255));
//  this->addNote(*new Note(79, &board5, 7, 0, 255));
//  this->addNote(*new Note(80, &board5, 8, 0, 255));
//  this->addNote(*new Note(81, &board5, 9, 0, 255));
//  this->addNote(*new Note(82, &board5, 10, 0, 255));
//  this->addNote(*new Note(83, &board5, 11, 0, 255));
//  this->addNote(*new Note(84, &board6, 0, 0, 255));
//  this->addNote(*new Note(85, &board6, 1, 0, 255));
//  this->addNote(*new Note(86, &board6, 2, 0, 255));
//  this->addNote(*new Note(87, &board6, 3, 0, 255));
//  this->addNote(*new Note(88, &board6, 4, 0, 255));
//  this->addNote(*new Note(89, &board6, 5, 0, 255));
//  this->addNote(*new Note(90, &board6, 6, 0, 255));
//  this->addNote(*new Note(91, &board6, 7, 0, 255));
//  this->addNote(*new Note(92, &board6, 8, 0, 255));
//  this->addNote(*new Note(93, &board6, 9, 0, 255));
//  this->addNote(*new Note(94, &board6, 10, 0, 255));
//  this->addNote(*new Note(95, &board6, 11, 0, 255));
//  this->addNote(*new Note(96, &board7, 0, 0, 255));
//  this->addNote(*new Note(97, &board7, 1, 0, 255));
//  this->addNote(*new Note(98, &board7, 2, 0, 255));
//  this->addNote(*new Note(99, &board7, 3, 0, 255));
//  this->addNote(*new Note(100, &board7, 4, 0, 255));
//  this->addNote(*new Note(101, &board7, 5, 0, 255));
//  this->addNote(*new Note(102, &board7, 6, 0, 255));
//  this->addNote(*new Note(103, &board7, 7, 0, 255));
//  this->addNote(*new Note(104, &board7, 8, 0, 255));
//  this->addNote(*new Note(105, &board7, 9, 0, 255));
//  this->addNote(*new Note(106, &board7, 10, 0, 255));
//  this->addNote(*new Note(107, &board7, 11, 0, 255));
//  this->addNote(*new Note(108, &board7, 12, 0, 255));

//  this->setSustainIsActive(false, millis());
//  this->setSustainBoard(&board7);
}
