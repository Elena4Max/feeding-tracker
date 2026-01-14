#include "ButtonStateMachine.h"

ButtonStateMachine::ButtonStateMachine() {
  currentState = IDLE;
  activeButton = "";
  startTime = 0;
  lastEventButton = "";
  lastStartTime = 0;
  lastEndTime = 0;
  finished = false;
  lastDebounceTime = 0;
}

State ButtonStateMachine::update(bool leftPressed, bool rightPressed) {
  finished = false;
  time_t now = time(nullptr);
  unsigned long currentMillis = millis();

  if (currentMillis - lastDebounceTime < debounceDelay)
    return currentState;
  lastDebounceTime = currentMillis;

  switch (currentState) {

    case IDLE:
      if (leftPressed) {
        startTime = now;
        activeButton = "LEFT";
        currentState = LEFT_ACTIVE;
        Serial.println("LEFT started");
      } 
      else if (rightPressed) {
        startTime = now;
        activeButton = "RIGHT";
        currentState = RIGHT_ACTIVE;
        Serial.println("RIGHT started");
      }
      break;

    case LEFT_ACTIVE:
      if (leftPressed && activeButton == "LEFT") {
        lastStartTime = startTime;
        lastEndTime   = now;
        lastEventButton = "LEFT";
        finished = true;

        currentState = IDLE;
        activeButton = "";
        Serial.println("LEFT stopped");
      } 
      else if (rightPressed && activeButton == "LEFT") {
        lastStartTime = startTime;
        lastEndTime   = now;
        lastEventButton = "LEFT";
        finished = true;
        Serial.println("LEFT stopped");

        startTime = now;
        activeButton = "RIGHT";
        currentState = RIGHT_ACTIVE;
        Serial.println("RIGHT started");
      }
      break;

    case RIGHT_ACTIVE:
      if (rightPressed && activeButton == "RIGHT") {
        lastStartTime = startTime;
        lastEndTime   = now;
        lastEventButton = "RIGHT";
        finished = true;

        currentState = IDLE;
        activeButton = "";
        Serial.println("RIGHT stopped");
      } 
      else if (leftPressed && activeButton == "RIGHT") {
        lastStartTime = startTime;
        lastEndTime   = now;
        lastEventButton = "RIGHT";
        finished = true;
        Serial.println("RIGHT stopped");

        startTime = now;
        activeButton = "LEFT";
        currentState = LEFT_ACTIVE;
        Serial.println("LEFT started");
      }
      break;
  }

  return currentState;
}

time_t ButtonStateMachine::getLastStartTime() { return lastStartTime; }
time_t ButtonStateMachine::getLastEndTime()   { return lastEndTime; }
String ButtonStateMachine::getLastEventButton() { return lastEventButton; }
bool ButtonStateMachine::eventFinished() { return finished; }
