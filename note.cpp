#include "note.h"
#include "piano.h"

#pragma once

int Note::getMidiId() { return this->midiId; }

int Note::calculateVelocity(int midiVelocity) {
  // read the pot value to set volume
  int potValue = analogRead(POT_PIN);
  // map a range of 0-4095 to a range of 0.5-1.5
  float velocityScale =  0.5 + ((1.5 - 0.5) / (4096.0)) * (potValue - 0.5);

  // map the midi range to the pwm range
  double slope = (NOTE_MAX_PWM - NOTE_MIN_PWM) / 128.0;
  int mappedVelocity = NOTE_MIN_PWM + round(slope * midiVelocity);

  // scale the volume
  int scaledVelocity = mappedVelocity * velocityScale;
  if(scaledVelocity > NOTE_MAX_PWM) {
    return NOTE_MAX_PWM;
  } else if(scaledVelocity < NOTE_MIN_PWM) {
    return NOTE_MIN_PWM;
  } else {
    return scaledVelocity;
  }
}

void Note::addToSchedule(int velocity) {
	unsigned long ms = millis();
	unsigned long msAndDelay = ms + FULL_DELAY;

	if(velocity > 0) //if note on command
	{
		int velocityMs = round(((-25 * velocity) / (double)127) + VELOCITY_MS);
		instances++;
		if(instances == 1) //if note is scheduled to deactivate (was 0 before instances++)
		{
			if(msAndDelay - velocityMs - MAX_STARTUP_MS >= schedule[OFF].back()) //if new note can be scheduled with current scheduling
			{
				schedule[STARTUP].   push_back(msAndDelay - velocityMs - MAX_STARTUP_MS);
				schedule[ACTIVATION].push_back(msAndDelay - velocityMs);
				schedule[ON].        push_back(msAndDelay);
				schedule[VELOCITY].  push_back(velocity);
				timeSinceActivation == ms;
			} else if(msAndDelay - MAX_DEACTIVATE_MS - velocityMs - MAX_STARTUP_MS >= schedule[ON].back()) //if current scheduling can be modified to still schedule the new note
			{
				schedule[DEACTIVATION].push_back(msAndDelay - velocityMs - MAX_STARTUP_MS - MAX_DEACTIVATE_MS);
				schedule[DEACTIVATION].erase(----schedule[DEACTIVATION].end());
				schedule[OFF].         push_back(msAndDelay - velocityMs - MAX_STARTUP_MS);
				schedule[OFF].         erase(----schedule[OFF].end());
				schedule[STARTUP].     push_back(msAndDelay - velocityMs - MAX_STARTUP_MS);
				schedule[ACTIVATION].  push_back(msAndDelay - velocityMs);
				schedule[ON].          push_back(msAndDelay);
				schedule[VELOCITY].    push_back(velocity);
				timeSinceActivation == ms;
			} else if(msAndDelay - FAST_DEACTIVATE_MS - velocityMs - MAX_STARTUP_MS >= schedule[ACTIVATION].back()) //if current scheduling can be modified with fast deactivation to schedule the new note
			{
				schedule[ON].          push_back(msAndDelay - velocityMs - MAX_STARTUP_MS - FAST_DEACTIVATE_MS);
				schedule[ON].          erase(----schedule[ON].end());
				schedule[DEACTIVATION].push_back(msAndDelay - velocityMs - MAX_STARTUP_MS - FAST_DEACTIVATE_MS);
				schedule[DEACTIVATION].erase(----schedule[DEACTIVATION].end());
				schedule[OFF].         push_back(msAndDelay - velocityMs - MAX_STARTUP_MS);
				schedule[OFF].         erase(----schedule[OFF].end());
				schedule[STARTUP].     push_back(msAndDelay - velocityMs - MAX_STARTUP_MS);
				schedule[ACTIVATION].  push_back(msAndDelay - velocityMs);
				schedule[ON].          push_back(msAndDelay);
				schedule[VELOCITY].    push_back(velocity);
				timeSinceActivation == ms;
			}
		} else //note is scheduled to activate and not deactivate
		{
			if(msAndDelay - MAX_DEACTIVATE_MS - velocityMs - MAX_STARTUP_MS >= schedule[ON].back()) //if current scheduling can be modified to still schedule the new note
			{
				schedule[DEACTIVATION].push_back(msAndDelay - velocityMs - MAX_STARTUP_MS - MAX_DEACTIVATE_MS);
				schedule[OFF].         push_back(msAndDelay - velocityMs - MAX_STARTUP_MS);
				schedule[STARTUP].     push_back(msAndDelay - velocityMs - MAX_STARTUP_MS);
				schedule[ACTIVATION].  push_back(msAndDelay - velocityMs);
				schedule[ON].          push_back(msAndDelay);
				schedule[VELOCITY].    push_back(velocity);
			} else if(msAndDelay - FAST_DEACTIVATE_MS - velocityMs - MAX_STARTUP_MS - 35 >= schedule[ACTIVATION].back() && schedule[ACTIVATION].back() > 0) //if current scheduling can be modified with fast deactivation to still schedule the new note
			{
				schedule[ON].          push_back(msAndDelay - velocityMs - MAX_STARTUP_MS - FAST_DEACTIVATE_MS);
				schedule[ON].          erase(----schedule[ON].end());
				schedule[DEACTIVATION].push_back(msAndDelay - velocityMs - MAX_STARTUP_MS - FAST_DEACTIVATE_MS);
				schedule[OFF].         push_back(msAndDelay - velocityMs - MAX_STARTUP_MS);
				schedule[STARTUP].     push_back(msAndDelay - velocityMs - MAX_STARTUP_MS);
				schedule[ACTIVATION].  push_back(msAndDelay - velocityMs);
				schedule[ON].          push_back(msAndDelay);
				schedule[VELOCITY].    push_back(velocity);
			}
		}
	} else if(instances > 0 /*&& velocity == 0*/) //if note off command and note is not already off
	{
		if(instances > 1) //if this isn't the last instance
		{
			//remove instance and exit
			instances--;
		} else //this is the last instance of the note and it should be scheduled
		{
			instances = 0;
			timeSinceActivation == 0;

			if(msAndDelay - FAST_DEACTIVATE_MS - 35 >= schedule[ACTIVATION].back() && msAndDelay - FAST_DEACTIVATE_MS - 35 <= schedule[ON].back() && schedule[ACTIVATION].back() > 0) //if it's efficient to use fast deactivation
			{
				schedule[ON].          push_back(msAndDelay - FAST_DEACTIVATE_MS);
				schedule[ON].          erase(----schedule[ON].end());
				schedule[DEACTIVATION].push_back(msAndDelay - FAST_DEACTIVATE_MS);
				schedule[OFF].         push_back(msAndDelay);
			} else if(msAndDelay - MAX_DEACTIVATE_MS >= schedule[ON].back()) //if regular deactivation works
			{
				schedule[DEACTIVATION].push_back(msAndDelay - MAX_DEACTIVATE_MS);
				schedule[OFF].         push_back(msAndDelay);
			} else //if all else fails the key shouldn't stay stuck on
			{
				if(schedule[ACTIVATION].back() > 0)
				{
					//immediately deactivate the key as soon as it makes sound
					schedule[ON].          push_back(schedule[ACTIVATION].back() + 35);
					schedule[ON].          erase(----schedule[ON].end());
					schedule[DEACTIVATION].push_back(schedule[ACTIVATION].back());
					schedule[OFF].         push_back(schedule[ACTIVATION].back() + FAST_DEACTIVATE_MS);
				} else //this should never happen
				{
					schedule[DEACTIVATION].push_back(msAndDelay);
					schedule[OFF].         push_back(msAndDelay + MAX_DEACTIVATE_MS);
				}
			}
		}
	}
}

void Note::checkForErrors() {
	unsigned long ms = millis();
	if(ms >= timeSinceActivation + NOTE_TIMEOUT_MS && timeSinceActivation > 0) resetSchedule();
	if(schedule[ON].size() > 1 && ms >= schedule[ON].at(1) + NOTE_TIMEOUT_MS) resetSchedule();
}

void Note::resetSchedule() {
	for(int index = 0; index < 6; index++) {
		schedule[index].resize(1);
		schedule[index].at(0) = 0;
	}

	schedule[OFF].push_back(millis());

	timeSinceActivation = 0;
	instances = 0;
  piano.addCommand(Command(this->getMidiId(), OFF_PWM));
}

void Note::processSchedule() {
	//reverse stack behavior: erases stack from the bottom
	unsigned long ms = millis();

	if(schedule[OFF].size() > 1 && schedule[DEACTIVATION].size() > 1 &&
		ms >= schedule[OFF].at(1) && schedule[OFF].at(1) >= schedule[DEACTIVATION].at(1)) //first because sometimes off and on times are the same
	{
		schedule[DEACTIVATION].erase(++schedule[DEACTIVATION].begin());
	}
	if(schedule[STARTUP].size() > 1 && schedule[OFF].size() > 1 && ms >= schedule[STARTUP].at(1) &&
		schedule[STARTUP].at(1) >= schedule[OFF].at(1))
	{
		schedule[OFF].erase(++schedule[OFF].begin());
    piano.addCommand(Command(this->getMidiId(), ON_PWM));
	}
	if(schedule[ACTIVATION].size() > 1 && schedule[STARTUP].size() > 1 && schedule[VELOCITY].size() > 1 &&
		ms >= schedule[ACTIVATION].at(1) && schedule[ACTIVATION].at(1) >= schedule[STARTUP].at(1))
	{
		schedule[STARTUP].erase(++schedule[STARTUP].begin());
    piano.addCommand(Command(this->getMidiId(), this->calculateVelocity(schedule[VELOCITY].at(1))));
	}
	if(schedule[ON].size() > 1 && schedule[ACTIVATION].size() > 1 &&
		ms >= schedule[ON].at(1) && schedule[ON].at(1) >= schedule[ACTIVATION].at(1))
	{
		schedule[ACTIVATION].erase(++schedule[ACTIVATION].begin());
		schedule[VELOCITY].erase(++schedule[VELOCITY].begin());
    piano.addCommand(Command(this->getMidiId(), NOTE_HOLD_PWM));
	}
	if(schedule[DEACTIVATION].size() > 1 && schedule[ON].size() > 1 &&
		ms >= schedule[DEACTIVATION].at(1) && schedule[DEACTIVATION].at(1) >= schedule[ON].at(1))
	{
		schedule[ON].erase(++schedule[ON].begin());
    piano.addCommand(Command(this->getMidiId(), OFF_PWM));
	}
}
