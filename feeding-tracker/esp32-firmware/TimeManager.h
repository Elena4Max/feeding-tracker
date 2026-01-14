#pragma once
#include <Arduino.h>   // ← ОБЯЗАТЕЛЬНО

class TimeManager {
public:
  bool setup(const String& timezone);
  bool isValid();
  String nowString();
};
