#pragma once
#include <WiFi.h>
#include "ConfigManager.h"

class WiFiSender {
public:
  bool connectWiFi(const Config& cfg);
  bool sendLogsEmail(const Config& cfg);
};
