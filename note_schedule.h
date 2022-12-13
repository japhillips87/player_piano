#import <string>

#pragma once

using namespace std;

class NoteSchedule {
  private:
    string type;
    unsigned long runAt;
    int velocity;

  public:
    NoteSchedule(string type, int velocity, unsigned long runAt)
      : type(type), velocity(velocity), runAt(runAt) {};
    string getType();
    int getVelocity();
    unsigned long getRunAt();
};
