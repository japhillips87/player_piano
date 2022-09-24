#include "command.h"

int Command::getPwm() { return this->pwm; }
int Command::getMidiId() { return this->midiId; }
unsigned long Command::getRunAt() { return this->runAt; }
