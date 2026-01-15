#pragma once
#include <Arduino.h>  

class TimeManager {
public:
  bool setup(const String& timezone);
  bool isValid();
  String nowString();
};
