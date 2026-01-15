#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "ConfigManager.h"
#include "TimeManager.h"

class SDLogger {
public:
  void init(const Config& cfg, TimeManager& tm);
  void logSession(const Config& cfg, time_t startT, time_t endT,
                  String breast);
  void clearLogs(const Config& cfg);

private:
  TimeManager* timeMgr = nullptr;
  const char* logFile = "/logs.csv";
  String formatTime(time_t t);
};
