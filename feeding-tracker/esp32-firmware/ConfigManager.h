#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

struct Config {
  String mother_name;
  String child_name;
  String dob;
  String pdr;
  String timezone;

  String email;
  unsigned long report_interval_min;

  String wifi_ssid;
  String wifi_password;
};

class ConfigManager {
public:
  bool loadConfig();
  Config getConfig();
private:
  Config config;
};
