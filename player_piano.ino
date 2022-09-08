#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32_NimBLE.h>
#include "schedule.h"
#include "piano.h"
#include "command.h"
#include <stdint.h>

Schedule schedule;
Piano piano;

// change to true to calibrate
const bool CALIBRATE = true;
const int BUTTON_PIN = 4;

BLEMIDI_CREATE_INSTANCE("Amadeus", MIDI)

void setup() {
  Serial.begin(115200);
  piano.initialize();
  schedule.poweredOn();
  MIDI.begin(MIDI_CHANNEL_OMNI);


  BLEMIDI.setHandleConnected([]() { schedule.connected(); });
  BLEMIDI.setHandleDisconnected([]() { schedule.disconnected(); });
  MIDI.setHandleNoteOn([](uint8_t _, uint8_t noteId, uint8_t velocity) { schedule.tryToScheduleNoteOn(noteId, velocity); });
  MIDI.setHandleNoteOff([](uint8_t _, uint8_t noteId, uint8_t velocity) { schedule.tryToScheduleNoteOff(noteId, velocity); });
  MIDI.setHandleControlChange([](uint8_t _, uint8_t number, uint8_t value) { schedule.tryToScheduleSustain(number, value); });

  if (CALIBRATE) {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    // uncomment one of the following to run calibration. do these in order
    // runStartupCalibration();
    // runVelocityValueCalibration();
    // runVelocityDurationCalibration();
    // runHoldCalibration();
    // runRepeatingCalibration();
  }
}

void loop() {
  MIDI.read();
  schedule.execute();
}

/*
 * CALIBRATION:
 * The purpose of calibration is to normalize the volume of the keys with each other, and to maximize
 * the speed at which you can play.
 *
 * If this calibration is done correctly, you should be able to play normalized volume songs at a speed faster than a human can play
 * without lag. Obviously, the piano cannot physically play faster than midi, but it should be able to play close to the limits of
 * the actual piano.
 *
 */

/*
 * For startup calibration, we are looking for the solenoid to start moving, but not actually hit the key. We want the solenoid to
 * move very quickly to start off with. Overdriving it will allow faster playing. The goal of this calibration is to find a value
 * where the solenoid moves and starts to actuate the key, without actually making any sound. This value is super important because
 * it will let us play really fast.
 */
void runStartupCalibration() {
  int buttonState = 0;
  unsigned long potPrintTime = millis();
  unsigned long noteScheduleTime = millis();
  schedule.execute();

  for (Note &note: piano.notes) {
    Serial.print("Calibrating note: ");
    Serial.println(note.getMidiId());
    delay(2000); // make sure there is enough time to release the button for the next note

    while (!buttonState) {
      int potValue = analogRead(A0);
      // map a range of 0-1023 to a range of 5-45
      float mappedValue =  5 + ((45 - 5) / (1023.0)) * (potValue - 5);

      if (millis() - potPrintTime >= 100) {
        Serial.print("Mapped pot value = ");
        Serial.println(mappedValue);
        potPrintTime = millis();
      }
      if (millis() - noteScheduleTime >= 1000) {
        schedule.commands.push_back(Command(note.getBoard(), note.getBoardIndex(), ON_PWM, millis()));
        schedule.commands.push_back(Command(note.getBoard(), note.getBoardIndex(), OFF_PWM, millis() + mappedValue));
        noteScheduleTime = millis();
      }

      buttonState = digitalRead(BUTTON_PIN);
    }
  }
}

/*
 * For velocity value calibration, you are looking for the minimum velocity where the note will sound and the maximum velocity
 * that you want for the final loudness of the piano.
 *
 * This should be set for each note. The pwm values provided in the midi file for velocity will be mapped within this range.
 */
void runVelocityValueCalibration() {
  int buttonState = 0;
  unsigned long potPrintTime = millis();
  unsigned long noteScheduleTime = millis();
  schedule.execute();

  for (Note &note: piano.notes) {
    Serial.print("Calibrating note: ");
    Serial.println(note.getMidiId());
    delay(2000); // make sure there is enough time to release the button for the next note

    while (!buttonState) {
      int potValue = analogRead(A0);
      // map a range of 0-1023 to a range of 0-255
      float mappedValue =  255 / 1023.0 * potValue;

      if (millis() - potPrintTime >= 100) {
        Serial.print("Mapped pot value = ");
        Serial.println(mappedValue);
        potPrintTime = millis();
      }
      if (millis() - noteScheduleTime >= 1000) {
        schedule.commands.push_back(Command(note.getBoard(), note.getBoardIndex(), ON_PWM, millis()));
        schedule.commands.push_back(Command(note.getBoard(), note.getBoardIndex(), mappedValue, millis() + STARTUP_DURATION));
        schedule.commands.push_back(Command(note.getBoard(), note.getBoardIndex(), OFF_PWM, millis() + STARTUP_DURATION + 500));
        noteScheduleTime = millis();
      }

      buttonState = digitalRead(BUTTON_PIN);
    }
  }
}

/*
 * For duration of velocity, you are looking for the lowest duration that gives you the same volume note. If the
 * duration gets too low, the note won't have the correct calibrated volume. If it gets too high, you won't be
 * able to play fast. Ideally, you will set this to the lowest value that will not affect final note volume.
 */
void runVelocityDurationCalibration() {
  int buttonState = 0;
  unsigned long potPrintTime = millis();
  unsigned long noteScheduleTime = millis();
  schedule.execute();

  for (Note &note: piano.notes) {
    Serial.print("Calibrating note: ");
    Serial.println(note.getMidiId());
    delay(2000); // make sure there is enough time to release the button for the next note

    while (!buttonState) {
      int potValue = analogRead(A0);
      // map a range of 0-1023 to a range of 0-200
      float mappedValue =  200 / 1023.0 * potValue;

      if (millis() - potPrintTime >= 100) {
        Serial.print("Mapped pot value = ");
        Serial.println(mappedValue);
        potPrintTime = millis();
      }
      if (millis() - noteScheduleTime >= 1000) {
        // TODO: we can't use this method. It will use hold velocities
        schedule.commands.push_back(Command(note.getBoard(), note.getBoardIndex(), ON_PWM, millis()));
        schedule.commands.push_back(Command(note.getBoard(), note.getBoardIndex(), MIDI_HALF_VELOCITY, millis() + STARTUP_DURATION));
        schedule.commands.push_back(Command(note.getBoard(), note.getBoardIndex(), OFF_PWM, millis() + STARTUP_DURATION + mappedValue));
        noteScheduleTime = millis();
      }

      buttonState = digitalRead(BUTTON_PIN);
    }
  }
}

/*
 * The purpose of the repeating calibration is to find the minimum deactivation time between notes.
 * If the deactivation time is too high, the same note cannot be played rapidly. If the time is too
 * low, the notes will fail to play twice in a row.
 */

void runRepeatingCalibration() {
  int buttonState = 0;
  unsigned long potPrintTime = millis();
  unsigned long noteScheduleTime = millis();
  schedule.execute();

  for (Note &note: piano.notes) {
    Serial.print("Calibrating note: ");
    Serial.println(note.getMidiId());
    delay(2000); // make sure there is enough time to release the button for the next note

    while (!buttonState) {
      int potValue = analogRead(A0);
      // map a range of 0-1023 to a range of 0-300
      float mappedValue =  300 / 1023.0 * potValue;

      if (millis() - potPrintTime >= 100) {
        Serial.print("Mapped pot value = ");
        Serial.println(mappedValue);
        potPrintTime = millis();
      }
      if (millis() - noteScheduleTime >= 1000) {
        schedule.scheduleNoteOnForDuration(note, note.calculateVelocity(MIDI_HALF_VELOCITY), millis(), TOTAL_ON_DURATION);
        schedule.scheduleNoteOnForDuration(note, note.calculateVelocity(MIDI_HALF_VELOCITY), millis() + mappedValue, TOTAL_ON_DURATION);
        noteScheduleTime = millis();
      }

      buttonState = digitalRead(BUTTON_PIN);
    }
  }
}
