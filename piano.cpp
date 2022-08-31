#include "piano.h"

Piano::Piano() { }

bool Piano::getSustainIsActive() { return this->sustainIsActive; }
unsigned long Piano::getSustainIsActiveSetAt() { return this->sustainIsActiveSetAt; }
PCA9635 *Piano::getSustainBoard() { return this->sustainBoard; }
void Piano::setSustainBoard(PCA9635 *board) { this->sustainBoard = board; }
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
  // TODO: add happy sound to play when the piano is initalized.
  // I think that Bb1-Bb2-D3-F3-Bb3-D4-F4-Bb4-D5-F5-Bb5 played quick then sustained would sound nice.
  // Device Orchestra uses a Bb chord like this and I like the way it sounds.
  PCA9635 board1(0x40);
  PCA9635 board2(0x42);
  PCA9635 board3(0x42);
  PCA9635 board4(0x43);
  PCA9635 board5(0x44);
  PCA9635 board6(0x45);
  PCA9635 board7(0x46);

  // TODO: this is very ugly
  Note note21(21, &board1, 0, 0, 255);
  Note note22(22, &board1, 1, 0, 255);
  Note note23(23, &board1, 2, 0, 255);
  Note note24(24, &board1, 3, 0, 255);
  Note note25(25, &board1, 4, 0, 255);
  Note note26(26, &board1, 5, 0, 255);
  Note note27(27, &board1, 6, 0, 255);
  Note note28(28, &board1, 7, 0, 255);
  Note note29(29, &board1, 8, 0, 255);
  Note note30(30, &board1, 9, 0, 255);
  Note note31(31, &board1, 10, 0, 255);
  Note note32(32, &board1, 11, 0, 255);
  Note note33(33, &board1, 12, 0, 255);
  Note note34(34, &board1, 13, 0, 255);
  Note note35(35, &board1, 14, 0, 255);
  Note note36(36, &board2, 1, 0, 255);
  Note note37(37, &board2, 2, 0, 255);
  Note note38(38, &board2, 3, 0, 255);
  Note note39(39, &board2, 4, 0, 255);
  Note note40(40, &board2, 5, 0, 255);
  Note note41(41, &board2, 7, 0, 255);
  Note note42(42, &board2, 8, 0, 255);
  Note note43(43, &board2, 9, 0, 255);
  Note note44(44, &board2, 10, 0, 255);
  Note note45(45, &board2, 11, 0, 255);
  Note note46(46, &board2, 12, 0, 255);
  Note note47(47, &board2, 13, 0, 255);
  Note note48(48, &board3, 0, 0, 255);
  Note note49(49, &board3, 1, 0, 255);
  Note note50(50, &board3, 2, 0, 255);
  Note note51(51, &board3, 3, 0, 255);
  Note note52(52, &board3, 4, 0, 255);
  Note note53(53, &board3, 5, 0, 255);
  Note note54(54, &board3, 6, 0, 255);
  Note note55(55, &board3, 7, 0, 255);
  Note note56(56, &board3, 8, 0, 255);
  Note note57(57, &board3, 9, 0, 255);
  Note note58(58, &board3, 10, 0, 255);
  Note note59(59, &board3, 11, 0, 255);
  Note note60(60, &board4, 0, 0, 255);
  Note note61(61, &board4, 1, 0, 255);
  Note note62(62, &board4, 2, 0, 255);
  Note note63(63, &board4, 3, 0, 255);
  Note note64(64, &board4, 4, 0, 255);
  Note note65(65, &board4, 5, 0, 255);
  Note note66(66, &board4, 6, 0, 255);
  Note note67(67, &board4, 7, 0, 255);
  Note note68(68, &board4, 8, 0, 255);
  Note note69(69, &board4, 9, 0, 255);
  Note note70(70, &board4, 10, 0, 255);
  Note note71(71, &board4, 11, 0, 255);
  Note note72(72, &board5, 0, 0, 255);
  Note note73(73, &board5, 1, 0, 255);
  Note note74(74, &board5, 2, 0, 255);
  Note note75(75, &board5, 3, 0, 255);
  Note note76(76, &board5, 4, 0, 255);
  Note note77(77, &board5, 5, 0, 255);
  Note note78(78, &board5, 6, 0, 255);
  Note note79(79, &board5, 7, 0, 255);
  Note note80(80, &board5, 8, 0, 255);
  Note note81(81, &board5, 9, 0, 255);
  Note note82(82, &board5, 10, 0, 255);
  Note note83(83, &board5, 11, 0, 255);
  Note note84(84, &board6, 0, 0, 255);
  Note note85(85, &board6, 1, 0, 255);
  Note note86(86, &board6, 2, 0, 255);
  Note note87(87, &board6, 3, 0, 255);
  Note note88(88, &board6, 4, 0, 255);
  Note note89(89, &board6, 5, 0, 255);
  Note note90(90, &board6, 6, 0, 255);
  Note note91(91, &board6, 7, 0, 255);
  Note note92(92, &board6, 8, 0, 255);
  Note note93(93, &board6, 9, 0, 255);
  Note note94(94, &board6, 10, 0, 255);
  Note note95(95, &board6, 11, 0, 255);
  Note note96(96, &board7, 0, 0, 255);
  Note note97(97, &board7, 1, 0, 255);
  Note note98(98, &board7, 2, 0, 255);
  Note note99(99, &board7, 3, 0, 255);
  Note note100(100, &board7, 4, 0, 255);
  Note note101(101, &board7, 5, 0, 255);
  Note note102(102, &board7, 6, 0, 255);
  Note note103(103, &board7, 7, 0, 255);
  Note note104(104, &board7, 8, 0, 255);
  Note note105(105, &board7, 9, 0, 255);
  Note note106(106, &board7, 10, 0, 255);
  Note note107(107, &board7, 11, 0, 255);
  Note note108(108, &board7, 12, 0, 255);
  this->addNote(note21);
  this->addNote(note22);
  this->addNote(note23);
  this->addNote(note24);
  this->addNote(note25);
  this->addNote(note26);
  this->addNote(note27);
  this->addNote(note28);
  this->addNote(note29);
  this->addNote(note30);
  this->addNote(note31);
  this->addNote(note32);
  this->addNote(note33);
  this->addNote(note34);
  this->addNote(note35);
  this->addNote(note36);
  this->addNote(note37);
  this->addNote(note38);
  this->addNote(note39);
  this->addNote(note40);
  this->addNote(note41);
  this->addNote(note42);
  this->addNote(note43);
  this->addNote(note44);
  this->addNote(note45);
  this->addNote(note46);
  this->addNote(note47);
  this->addNote(note48);
  this->addNote(note49);
  this->addNote(note50);
  this->addNote(note51);
  this->addNote(note52);
  this->addNote(note53);
  this->addNote(note54);
  this->addNote(note55);
  this->addNote(note56);
  this->addNote(note57);
  this->addNote(note58);
  this->addNote(note59);
  this->addNote(note60);
  this->addNote(note61);
  this->addNote(note62);
  this->addNote(note63);
  this->addNote(note64);
  this->addNote(note65);
  this->addNote(note66);
  this->addNote(note67);
  this->addNote(note68);
  this->addNote(note69);
  this->addNote(note70);
  this->addNote(note71);
  this->addNote(note72);
  this->addNote(note73);
  this->addNote(note74);
  this->addNote(note75);
  this->addNote(note76);
  this->addNote(note77);
  this->addNote(note78);
  this->addNote(note79);
  this->addNote(note80);
  this->addNote(note81);
  this->addNote(note82);
  this->addNote(note83);
  this->addNote(note84);
  this->addNote(note85);
  this->addNote(note86);
  this->addNote(note87);
  this->addNote(note88);
  this->addNote(note89);
  this->addNote(note90);
  this->addNote(note91);
  this->addNote(note92);
  this->addNote(note93);
  this->addNote(note94);
  this->addNote(note95);
  this->addNote(note96);
  this->addNote(note97);
  this->addNote(note98);
  this->addNote(note99);
  this->addNote(note100);
  this->addNote(note101);
  this->addNote(note102);
  this->addNote(note103);
  this->addNote(note104);
  this->addNote(note105);
  this->addNote(note106);
  this->addNote(note107);
  this->addNote(note108);

  this->setSustainIsActive(false, millis());
  this->setSustainBoard(&board1);
}
