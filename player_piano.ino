#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32_NimBLE.h>
#include "schedule.h"
#include "piano.h"
#include <stdint.h>

Schedule schedule;
Piano piano;

BLEMIDI_CREATE_INSTANCE("Amadeus", MIDI)

void setup() {
  Serial.begin(115200);
  piano.initialize();
  schedule.poweredOn();
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // TODO: add sound for bluetooth connected and disconnected.
  // i suggest D4-A4-D5 with 50 millis between note starts and 500 millis duration.
  BLEMIDI.setHandleConnected([]() { schedule.connected(); });
  BLEMIDI.setHandleDisconnected([]() { schedule.disconnected(); });
  MIDI.setHandleNoteOn([](uint8_t _, uint8_t noteId, uint8_t velocity) { schedule.tryToScheduleNoteOn(noteId, velocity); });
  MIDI.setHandleNoteOff([](uint8_t _, uint8_t noteId, uint8_t velocity) { schedule.tryToScheduleNoteOff(noteId, velocity); });
  MIDI.setHandleControlChange([](uint8_t _, uint8_t number, uint8_t value) { schedule.tryToScheduleSustain(number, value); });

  //runCalibration();
  //runTest();
}

void runTest() {
  // TODO: loop through piano notes and play each one, first ascending, then descending.
}

/*
 * The purpose of calibration is to normalize the volume of the keys with each other, and to maximize
 * the speed at which you can play.
 *
 * The calibrate function is used to test the values that you need to set for min and max velocity,
 * and the duration for each phase of each key.
 *
 * For velocity, you are looking for the minimum velocity where the note will sound and the maximum velocity that you want.
 * This should be set for each note. The pwm values provided in the midi file for velocity will be mapped within this range.
 *
 * For duration of phases, you are looking for the quickest action. Ideally, you want the solenoid to start really fast,
 * but only long enough to not affect final velocity. You also want the velocity phase to only last as long as it needs to
 * last without affecting final velocity. For off duration, you are looking for the fastest speed at which you can play the
 * same note back to back with both of the notes sounding correct.
 *
 * If this calibration is done correctly, you should be able to play normalized volume songs at a speed faster than a human can play
 * without lag. Obviously, the piano cannot physically play faster than midi, but it should be able to play close to the limits of
 * the actual piano.
 *
 */
void runCalibration() {
  // TODO: use a button to advance to the next key and a potentiometer to adjust the value of the parameter under calibration
  // everything should be logged to the serial monitor. use a delay of millis() to constanly print out pot value without hitting the key
  // too often.

  // Uncomment one of the following to run each phase of calibration. Each note should be updated between phases and they should be run in order
  int calibrateMode = 1; // calibrate velocity
  //int calibrateMode = 2; // calibrate startup duration
  //int calibrateMode = 3 // calibrate velocity duration
  //int calibrateMode = 4 // calibrate off duration
  switch(calibrateMode) {
    case 1:
      // loop through notes and vary velocity with no startup or hold durations or velocities
      break;
    case 2:
      // loop through notes with calibrated 50% velocity and varying startup duration and no hold velocity or duration
      break;
    case 3:
      // loop through notes with calibrated 50% velocity and calibrated startup duration with varying velocity duration
      break;
    case 4:
      // loop through notes with calibrated values from above 3 steps and decreasing time between notes.
      break;
  }
}

void loop() {
  MIDI.read();
  schedule.execute();
}
