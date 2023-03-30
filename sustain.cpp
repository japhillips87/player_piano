#include "sustain.h"
#include "settings.h"
#include "piano.h"

Sustain::Sustain() {
  //initialize one row of vectors so program won't crash while comparing
  //initialize sustain as off by default
  for(int index = 0; index < 4; index++)
  {
    schedule[index].reserve(6);
    schedule[index].resize(1);
  }
  schedule[OFF].push_back(millis());
}

void Sustain::addToSchedule(uint8_t velocity) {
  if(velocity < SUSTAIN_THRESHOLD) {
    scheduleSustain(false);
  } else if(instances == 0) { //only schedule if sustain is off
    scheduleSustain(true);
  }
}

void Sustain::processSchedule() {
  unsigned long ms = millis();
  //less checks for sustain because it's slower and less important
  if(schedule[OFF].size() > 1 && schedule[DEACTIVATION].size() > 1 &&
    ms >= schedule[OFF].at(1) && schedule[OFF].at(1) >= schedule[DEACTIVATION].at(1))
  {
    schedule[DEACTIVATION].erase(schedule[DEACTIVATION].begin()++);
    piano.addCommand(Command(SUSTAIN_COMMAND_ID, OFF_PWM));
  }
  if(schedule[ACTIVATION].size() > 1 && schedule[OFF].size() > 1 &&
    ms >= schedule[ACTIVATION].at(1) && schedule[ACTIVATION].at(1) >= schedule[OFF].at(1))
  {
    schedule[OFF].erase(schedule[OFF].begin()++);
    piano.addCommand(Command(SUSTAIN_COMMAND_ID, ON_PWM));
  }
  if(schedule[ON].size() > 1 && schedule[ACTIVATION].size() > 1 &&
    ms >= schedule[ON].at(1) && schedule[ON].at(1) >= schedule[ACTIVATION].at(1))
  {
    schedule[ACTIVATION].erase(schedule[ACTIVATION].begin()++);
    piano.addCommand(Command(SUSTAIN_COMMAND_ID, SUSTAIN_HOLD_PWM));
  }
  if(schedule[DEACTIVATION].size() > 1 && schedule[ON].size() > 1 &&
    ms >= schedule[DEACTIVATION].at(1) && schedule[DEACTIVATION].at(1) >= schedule[ON].at(1))
  {
    schedule[ON].erase(schedule[ON].begin()++);
    piano.addCommand(Command(SUSTAIN_COMMAND_ID, OFF_PWM));
  }
}

void Sustain::checkForErrors() {
  unsigned long ms = millis();
  if(ms >= timeSinceActivation + SUSTAIN_TIMEOUT_MS && timeSinceActivation > 0) resetSchedule();
  if(schedule[ON].size() > 1) if(ms >= schedule[ON].at(1) + SUSTAIN_TIMEOUT_MS) resetSchedule();
}

void Sustain::resetSchedule() {
  for(int index = 0; index < 4; index++) {
    schedule[index].resize(1);
  }
  schedule[OFF].push_back(millis());
  timeSinceActivation = 0;
  instances = 0;
  piano.addCommand(Command(SUSTAIN_COMMAND_ID, OFF_PWM));
}

void Sustain::scheduleSustain(bool state)
{
  unsigned long ms = millis();
  unsigned long msAndDelay = ms + FULL_DELAY;

  if(state)
  {
    if(msAndDelay - SUSTAIN_ON_MS >= schedule[OFF].back()) //if sustain can be scheduled with current scheduling
    {
      schedule[ACTIVATION].push_back(msAndDelay - SUSTAIN_ON_MS);
      schedule[ON].        push_back(msAndDelay);
      timeSinceActivation = ms;
      instances++;
    } else if(msAndDelay - SUSTAIN_OFF_MS - SUSTAIN_ON_MS >= schedule[ON].back()) //if current scheduling can be modified to still schedule the sustain
    {
      schedule[DEACTIVATION].push_back(msAndDelay - SUSTAIN_ON_MS - SUSTAIN_OFF_MS);
      schedule[DEACTIVATION].erase(----schedule[DEACTIVATION].end());
      schedule[OFF].         push_back(msAndDelay - SUSTAIN_ON_MS);
      schedule[OFF].         erase(----schedule[OFF].end());
      schedule[ACTIVATION]  .push_back(msAndDelay - SUSTAIN_OFF_MS);
      schedule[ON].          push_back(msAndDelay);
      timeSinceActivation = ms;
      instances++;
    }
  } else if(instances > 0) //if sustain off command and sustain is not already off
  {
    instances = 0;
    if(msAndDelay - SUSTAIN_OFF_MS >= schedule[ON].back()) //if sustain can be ideally deactivated
    {
      schedule[DEACTIVATION].push_back(msAndDelay - SUSTAIN_OFF_MS);
      schedule[OFF].         push_back(msAndDelay);
    } else //deactivate sustain anyways so it's not stuck on
    {
      schedule[DEACTIVATION].push_back(msAndDelay);
      schedule[OFF].         push_back(msAndDelay + SUSTAIN_OFF_MS);
    }
  }
}
