#include "command.h"

Command::Command(PCA9635 *board, int i2cIndex, int pwm, unsigned long runAt) {
  this->board = board;
  this->i2cIndex = i2cIndex;
  this->pwm = pwm;
  this->runAt = runAt;
}

PCA9635 *Command::getBoard() { return this->board; }
int Command::getI2cIndex() { return this->i2cIndex; }
int Command::getPwm() { return this->pwm; }
unsigned long Command::getRunAt() { return this->runAt; }
