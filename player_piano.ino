#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32_NimBLE.h>
#include "schedule.h"
#include "piano.h"
#include "command.h"
#include <stdint.h>

Schedule schedule;
Piano piano;
PCA9635 board1(0x40);
PCA9635 board2(0x41);
PCA9635 board3(0x42);
PCA9635 board4(0x43);
PCA9635 board5(0x44);
PCA9635 board6(0x45);
PCA9635 board7(0x46);

const int potPin = 36;

BLEMIDI_CREATE_INSTANCE("Amadeus", MIDI)

void setup() {
  Serial.begin(115200);
  MIDI.begin(MIDI_CHANNEL_OMNI);

//  BLEMIDI.setHandleConnected([]() { schedule.connected(); });
//  BLEMIDI.setHandleDisconnected([]() { schedule.disconnected(); });
  MIDI.setHandleNoteOn([](uint8_t _, uint8_t noteId, uint8_t velocity) { schedule.tryToScheduleNoteOn(noteId, velocity); });
  MIDI.setHandleNoteOff([](uint8_t _, uint8_t noteId, uint8_t velocity) { schedule.tryToScheduleNoteOff(noteId, velocity); });
  MIDI.setHandleControlChange([](uint8_t _, uint8_t number, uint8_t value) { schedule.tryToScheduleSustain(number, value); });

  piano.initialize();
//  schedule.poweredOn();

  board1.begin(SDA_PIN, SCL_PIN, PCA9635_MODE1_NONE, PCA9635_MODE2_INVERT | PCA9635_MODE2_TOTEMPOLE);
  for (int channel = 0; channel < board1.channelCount(); channel++) {
    board1.setLedDriverMode(channel, PCA9635_LEDPWM);
    board1.write1(channel, 0);
  }
//  board2.begin(SDA_PIN, SCL_PIN, PCA9635_MODE1_NONE, PCA9635_MODE2_INVERT | PCA9635_MODE2_TOTEMPOLE);
//  for (int channel = 0; channel < board2.channelCount(); channel++) {
//    board2.setLedDriverMode(channel, PCA9635_LEDPWM);
//    board2.write1(channel, 0);
//  }
//  board3.begin(SDA_PIN, SCL_PIN, PCA9635_MODE1_NONE, PCA9635_MODE2_INVERT | PCA9635_MODE2_TOTEMPOLE);
//  for (int channel = 0; channel < board3.channelCount(); channel++) {
//    board3.setLedDriverMode(channel, PCA9635_LEDPWM);
//    board3.write1(channel, 0);
//  }
//  board4.begin(SDA_PIN, SCL_PIN, PCA9635_MODE1_NONE, PCA9635_MODE2_INVERT | PCA9635_MODE2_TOTEMPOLE);
//  for (int channel = 0; channel < board4.channelCount(); channel++) {
//    board4.setLedDriverMode(channel, PCA9635_LEDPWM);
//    board4.write1(channel, 0);
//  }
//  board5.begin(SDA_PIN, SCL_PIN, PCA9635_MODE1_NONE, PCA9635_MODE2_INVERT | PCA9635_MODE2_TOTEMPOLE);
//  for (int channel = 0; channel < board5.channelCount(); channel++) {
//    board5.setLedDriverMode(channel, PCA9635_LEDPWM);
//    board5.write1(channel, 0);
//  }
//  board6.begin(SDA_PIN, SCL_PIN, PCA9635_MODE1_NONE, PCA9635_MODE2_INVERT | PCA9635_MODE2_TOTEMPOLE);
//  for (int channel = 0; channel < board6.channelCount(); channel++) {
//    board6.setLedDriverMode(channel, PCA9635_LEDPWM);
//    board6.write1(channel, 0);
//  }
//  board7.begin(SDA_PIN, SCL_PIN, PCA9635_MODE1_NONE, PCA9635_MODE2_INVERT | PCA9635_MODE2_TOTEMPOLE);
//  for (int channel = 0; channel < board7.channelCount(); channel++) {
//    board7.setLedDriverMode(channel, PCA9635_LEDPWM);
//    board7.write1(channel, 0);
//  }

  // uncomment one of the following to run calibration. do these in order
  // runStartupCalibration();
  // runVelocityValueCalibration();
  // runHoldCalibration();
  // runVelocityDurationCalibration();
  // runRepeatingCalibration();
}

void loop() {
  MIDI.read();
  // loop through the notes and and see if their schedule needs to be adjusted
  for (int midiId = 21; midiId <= 108; midiId++) {
    Note &note = piano.find(midiId);
  }

  // loop through the commands and find which ones need to run
  for (auto it = schedule.commands.begin(); it != schedule.commands.end(); it++) {
    if (it->getRunAt() <= millis()) {
      int midiId = it->getMidiId();
      if (midiId >= BOARD_1_MIN_ID && midiId <= BOARD_1_MAX_ID) {
        board1.write1(midiId - BOARD_1_MIN_ID, it->getPwm());
//      } else if (midiId >= BOARD_2_MIN_ID && midiId <= BOARD_2_MAX_ID) {
//        board2.write1(midiId - BOARD_2_MIN_ID, it->getPwm());
//      } else if (midiId >= BOARD_3_MIN_ID && midiId <= BOARD_3_MAX_ID) {
//        board3.write1(midiId - BOARD_3_MIN_ID, it->getPwm());
//      } else if (midiId >= BOARD_4_MIN_ID && midiId <= BOARD_4_MAX_ID) {
//        board4.write1(midiId - BOARD_4_MIN_ID, it->getPwm());
//      } else if (midiId >= BOARD_5_MIN_ID && midiId <= BOARD_5_MAX_ID) {
//        board5.write1(midiId - BOARD_5_MIN_ID, it->getPwm());
//      } else if (midiId >= BOARD_6_MIN_ID && midiId <= BOARD_6_MAX_ID) {
//        board6.write1(midiId - BOARD_6_MIN_ID, it->getPwm());
//      } else if (midiId >= BOARD_7_MIN_ID && midiId <= BOARD_7_MAX_ID) {
//        board7.write1(midiId - BOARD_7_MIN_ID, it->getPwm());
      }
      Serial.print("RUNNING COMMAND: ");
      Serial.print("Midi Id: ");
      Serial.print(it->getMidiId());
      Serial.print(", PWM: ");
      Serial.println(it->getPwm());
      schedule.commands.erase(it--);
    }
  }
}

/*
 * CALIBRATION:
 * The purpose of calibration is to normalize the volume of the keys with each other, and to maximize
 * the speed at which you can play.
 *
 * If this calibration is done correctly, you should be able to play normalized volume songs at a speed faster than a human can play
 * without lag. Obviously, the piano cannot physically play faster than midi, but it should be able to play close to the limits of
 * the actual piano. The calibration is only for one note. In my case, I'm going to just use the very first note on the very first board.
 *
 */

/*
 * For startup calibration, we are looking for the solenoid to start moving, but not actually hit the key. We want the solenoid to
 * move very quickly to start off with. Overdriving it will allow faster playing. The goal of this calibration is to find a value
 * where the solenoid moves and starts to actuate the key, without actually making any sound. This value is super important because
 * it will let us play really fast.
 */

void runStartupCalibration() {
  while(true) {
    int potValue = analogRead(potPin);
    // map a range of 0-4095 to a range of 5-45
    float mappedValue =  5 + ((45 - 5) / (4096.0)) * (potValue - 5);

    Serial.print("Mapped pot value = ");
    Serial.println(mappedValue);

    board1.write1(0, ON_PWM);
    delay(mappedValue);
    board1.write1(0, OFF_PWM);

    delay(2000);
  }
}

/*
 * For velocity value calibration, you are looking for the minimum velocity where the note will sound and the maximum velocity
 * that you want for the final loudness of the piano.
 *
 * This should be set for each note. The pwm values provided in the midi file for velocity will be mapped within this range.
 */

void runVelocityValueCalibration() {
  while (true) {
    int potValue = analogRead(potPin);
    // map a range of 0-4095 to a range of 0-255
    float mappedValue =  255 / 4096.0 * potValue;

    Serial.print("Mapped pot value = ");
    Serial.println(mappedValue);

    board1.write1(0, ON_PWM);
    delay(STARTUP_DURATION);
    board1.write1(0, mappedValue);
    delay(500);
    board1.write1(0, OFF_PWM);

    delay(2000);
  }
}

/*
 * For hold calibration, you are looking for the minimum pwm value where the note will hold.
 * You want to thin increase this just a bit more so that it will always hold.
 */

void runHoldCalibration() {
  while (true) {
    int potValue = analogRead(potPin);
    // map a range of 0-4095 to a range of 0-64
    float mappedValue =  64 / 4096.0 * potValue;

    Serial.print("Mapped pot value = ");
    Serial.println(mappedValue);

    board1.write1(0, ON_PWM);
    delay(STARTUP_DURATION);
    board1.write1(0, NOTE_MID_PWM);
    delay(500);
    board1.write1(0, mappedValue);
    delay(500);
    board1.write1(0, OFF_PWM);

    delay(2000);
  }
}

// /*
//  * For duration of velocity, you are looking for the lowest duration that gives you the same volume note. If the
//  * duration gets too low, the note won't have the correct calibrated volume. If it gets too high, you won't be
//  * able to play fast. Ideally, you will set this to the lowest value that will not affect final note volume.
//  */

void runVelocityDurationCalibration() {
  while (true) {
    int potValue = analogRead(potPin);
    // map a range of 0-4095 to a range of 0-200
    float mappedValue =  200 / 4096.0 * potValue;

    Serial.print("Mapped pot value = ");
    Serial.println(mappedValue);

    board1.write1(0, ON_PWM);
    delay(STARTUP_DURATION);
    board1.write1(0, NOTE_MID_PWM);
    delay(mappedValue);
    board1.write1(0, NOTE_HOLD_PWM);
    delay(500);
    board1.write1(0, OFF_PWM);

    delay(2000);
  }
}

// /*
//  * The purpose of the repeating calibration is to find the minimum deactivation time between notes.
//  * If the deactivation time is too high, the same note cannot be played rapidly. If the time is too
//  * low, the notes will fail to play twice in a row.
//  */

void runRepeatingCalibration() {
  while (true) {
    int potValue = analogRead(potPin);
    // map a range of 0-4095 to a range of 0-300
    float mappedValue =  300 / 4096.0 * potValue;

    Serial.print("Mapped pot value = ");
    Serial.println(mappedValue);

    board1.write1(0, ON_PWM);
    delay(STARTUP_DURATION);
    board1.write1(0, NOTE_MID_PWM);
    delay(VELOCITY_DURATION);
    board1.write1(0, OFF_PWM);
    delay(DEACTIVATE_TIME_REQUIRED);

    board1.write1(0, ON_PWM);
    delay(STARTUP_DURATION);
    board1.write1(0, NOTE_MID_PWM);
    delay(VELOCITY_DURATION);
    board1.write1(0, OFF_PWM);

    delay(2000);
  }
}
