

//// https://gist.github.com/RyoKosaka/9851abbf0393aa6790a14c4806656f04
//
//#include <BLEMIDI_Transport.h>
//#include <hardware/BLEMIDI_ESP32.h>
//
//BLEMIDI_CREATE_DEFAULT_INSTANCE()
//
//void setup() {
//  MIDI.begin();
//}
//
//void loop() {
//  uint8_t note;
//  uint8_t velocity;
//
//  if (MIDI.read()) { //when get MIDI signal
//    switch (MIDI.getType()) {
//      case midi::NoteOn:
//        note = MIDI.getData1();
//        velocity = MIDI.getData2();
//        tryToScheduleNote("on", data1, data2);
//        break;
//      case midi::NoteOff:
//        note = MIDI.getData1();
//        velocity = MIDI.getData2();
//        tryToScheduleNote("off", data1, data2);
//        break;
//      case midi::ControlChange:
//        note = MIDI.getData1();
//        velocity = MIDI.getData2();
//        if (note == 64) {
//          tryToScheduleSustain(velocity);
//        }
//    }
//  }
//}

#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32_NimBLE.h>
#include "schedule.h"
#include <stdint.h>

Schedule schedule;

BLEMIDI_CREATE_INSTANCE("Amadeus", MIDI)

void setup() {
  Serial.begin(115200);
  MIDI.begin();
    
  MIDI.setHandleNoteOn([](uint8_t _, uint8_t noteId, uint8_t velocity) { schedule.tryToScheduleNoteOn(noteId, velocity); });
  MIDI.setHandleNoteOff([](uint8_t _, uint8_t noteId, uint8_t velocity) { schedule.tryToScheduleNoteOff(noteId, velocity); });
  MIDI.setHandleControlChange([](uint8_t _, uint8_t number, uint8_t value) { schedule.tryToScheduleSustain(number, value); });
}

// TODO: create a test loop to find note min and max velocities.
// would be best to set a flag to toggle bettween the tesing and actual loop.
void loop() {
  MIDI.read();
  schedule.execute();
}
