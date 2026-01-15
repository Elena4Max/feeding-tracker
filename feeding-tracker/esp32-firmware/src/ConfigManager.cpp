#include "../include/ConfigManager.h"
#include <SD.h>

bool ConfigManager::loadConfig() {
  if (!SD.exists("/config.json")) {
    Serial.println("config.json not found!");
    return false;
  }

  File f = SD.open("/config.json");
  if (!f) {
    Serial.println("Failed to open config.json");
    return false;
  }

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();

  if (error) {
    Serial.print("Failed to parse config.json: ");
    Serial.println(error.c_str());
    return false;
  }

  config.mother_name = doc["mother_name"] | "Unknown";
  config.child_name = doc["child_name"] | "Unknown";
  config.dob = doc["dob"] | "";
  config.pdr = doc["pdr"] | "";
  config.timezone = doc["timezone"] | "UTC";
  config.email = doc["email"] | "";
  config.report_interval_min = doc["report_interval_min"] | 30;
  config.wifi_ssid     = doc["wifi_ssid"] | "";
  config.wifi_password = doc["wifi_password"] | "";

  Serial.print("WiFi SSID: ");
  Serial.println(config.wifi_ssid);

  Serial.println("Config loaded:");
  Serial.println(config.child_name);
  Serial.println(config.email);
  Serial.print("Report interval (min): "); Serial.println(config.report_interval_min);

  return true;
}

Config ConfigManager::getConfig() {
  return config;
}
