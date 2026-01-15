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
  String activeButton;
  time_t startTime;

  String lastEventButton;
  time_t lastStartTime;
  time_t lastEndTime;
  bool finished;

  unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 1000; 

public:
  ButtonStateMachine();

  State update(bool leftPressed, bool rightPressed);

  time_t getLastStartTime();
  time_t getLastEndTime();
  String getLastEventButton();
  bool eventFinished();
};
