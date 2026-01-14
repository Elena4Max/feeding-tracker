#pragma once
#include <Arduino.h>
#include <time.h>

enum State {
  IDLE,
  LEFT_ACTIVE,
  RIGHT_ACTIVE
};

class ButtonStateMachine {
private:
  State currentState;
  time_t startTime;

public:
  ButtonStateMachine();
  
  State update(bool leftPressed, bool rightPressed);

  time_t getStartTime();
  time_t getEndTime();

  bool eventFinished();
  const char* getEventButton();
};
