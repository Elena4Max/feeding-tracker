#include "ButtonStateMachine.h"
#include <time.h>

ButtonStateMachine::ButtonStateMachine() {
  currentState = IDLE;
  startTime = 0;
}

// внутренние переменные
static time_t endTime = 0;
static const char* eventButton = nullptr;
static bool finished = false;

State ButtonStateMachine::update(bool leftPressed, bool rightPressed) {
  finished = false;

  time_t now = time(nullptr);   // ← ВАЖНО

  switch (currentState) {

    case IDLE:
      if (leftPressed) {
        startTime = now;
        currentState = LEFT_ACTIVE;
        Serial.println("LEFT started");
        delay(200);
      }
      else if (rightPressed) {
        startTime = now;
        currentState = RIGHT_ACTIVE;
        Serial.println("RIGHT started");
        delay(200);
      }
      break;

    case LEFT_ACTIVE:
      if (leftPressed || rightPressed) {
        endTime = now;
        eventButton = leftPressed ? "LEFT" : "RIGHT";
        finished = true;
        currentState = IDLE;
        delay(200);
      }
      break;

    case RIGHT_ACTIVE:
      if (leftPressed || rightPressed) {
        endTime = now;
        eventButton = rightPressed ? "RIGHT" : "LEFT";
        finished = true;
        currentState = IDLE;
        delay(200);
      }
      break;
  }

  return currentState;
}

time_t ButtonStateMachine::getStartTime() { return startTime; }
time_t ButtonStateMachine::getEndTime()   { return endTime; }
const char* ButtonStateMachine::getEventButton() { return eventButton; }
bool ButtonStateMachine::eventFinished() { return finished; }
