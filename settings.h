#pragma once

const int OFF_PWM = 0;
const int ON_PWM = 255;
const int STARTUP_DURATION = 12;
const int VELOCITY_DURATION = 40;
const int TOTAL_ON_DURATION = STARTUP_DURATION + VELOCITY_DURATION;
const int NOTE_HOLD_PWM = 65;
const int NOTE_MIN_PWM = 70;
const int NOTE_MAX_PWM = 160;
const int NOTE_MID_PWM = (NOTE_MIN_PWM + NOTE_MAX_PWM) / 2;
const int MIN_MIDI_VELOCITY = 0;
const int MAX_MIDI_VELOCITY = 127;
const int MIDI_HALF_VELOCITY = (MIN_MIDI_VELOCITY + MAX_MIDI_VELOCITY) / 2;
// const int SUSTAIN_STARTUP_DURATION = 30;
// const int SUSTAIN_POWER_DURATION = 100;
// const int SUSTAIN_TOTAL_ON_DURATION = SUSTAIN_STARTUP_DURATION + SUSTAIN_POWER_DURATION;
// const int SUSTAIN_VELOCITY = 127;
// const int SUSTAIN_HOLD_PWM = 50;
// const int SUSTAIN_DEACTIVATE_TIME_REQUIRED = 200;
const int SDA_PIN = 21;
const int SCL_PIN = 22;
const int MIN_STARTUP_MS     = 18;
const int MAX_STARTUP_MS     = 18;
const int VELOCITY_MS        = 35;
const int MIN_DEACTIVATE_MS  = 75;
const int MAX_DEACTIVATE_MS  = 80;
const int FAST_DEACTIVATE_MS = 52;
const int SUSTAIN_ON_MS      = 91;
const int SUSTAIN_OFF_MS     = 50;
const int NOTE_TIMEOUT_MS    = 10000;
const int SUSTAIN_TIMEOUT_MS = 30000;
const int AUTO_RESET_MS      = 360000;
static int noteVelocityMs[127];
// Delay Settings
const int FULL_DELAY = MAX_STARTUP_MS + VELOCITY_MS + MAX_DEACTIVATE_MS;
// BOARD_MIDI_VALUES
const int BOARD_1_MIN_ID = 21;
const int BOARD_1_MAX_ID = 35;
const int BOARD_2_MIN_ID = 36;
const int BOARD_2_MAX_ID = 47;
const int BOARD_3_MIN_ID = 48;
const int BOARD_3_MAX_ID = 59;
const int BOARD_4_MIN_ID = 60;
const int BOARD_4_MAX_ID = 71;
const int BOARD_5_MIN_ID = 72;
const int BOARD_5_MAX_ID = 83;
const int BOARD_6_MIN_ID = 84;
const int BOARD_6_MAX_ID = 95;
const int BOARD_7_MIN_ID = 96;
const int BOARD_7_MAX_ID = 108;
// TODO: calibrate these values
const int BLUETOOTH_SOUND_DELAY = 75;
const int BLUETOOTH_SOUND_DURATION = 300;
const int POWER_SOUND_DURATION_ONE = 200;
const int POWER_SOUND_DURATION_TWO = 1000;
const int POWER_SOUND_DELAY = 100;
