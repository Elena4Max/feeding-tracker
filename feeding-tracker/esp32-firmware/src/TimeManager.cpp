#include <Arduino.h>
#include "../include/TimeManager.h"
#include <time.h>

bool TimeManager::setup(const String& timezone) {

  Serial.print("Timezone: ");
  Serial.println(timezone);

  configTzTime(
    timezone.c_str(),
    "pool.ntp.org",
    "time.nist.gov"
  );

  return true;
}

bool TimeManager::isValid() {
  time_t now = time(nullptr);
  return now > 1600000000;
}

String TimeManager::nowString() {
  time_t now;
  time(&now);

  struct tm t;
  localtime_r(&now, &t);

  char buf[32];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
  return String(buf);
}
