#include <Arduino.h>

#include "config.h"
#include "ConfigManager.h"
#include "ButtonStateMachine.h"
#include "SDLogger.h"
#include "WiFiSender.h"
#include "TimeManager.h"

ConfigManager cfgMgr;
Config cfg;

ButtonStateMachine fsm;
SDLogger sdLogger;
WiFiSender wifiSender;
TimeManager timeMgr;

time_t lastSendTime = 0;
time_t sendIntervalSec = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  sdLogger.init(cfg, timeMgr);

  if (!cfgMgr.loadConfig()) {
    Serial.println("Config load failed");
  }
  cfg = cfgMgr.getConfig();

  wifiSender.connectWiFi(cfg);

  timeMgr.setup(cfg.timezone);
  
  while (!timeMgr.isValid()) {
    delay(500);
  }

  sendIntervalSec = cfg.report_interval_min * 60;
}

void loop() {
  bool leftPressed  = digitalRead(BTN_LEFT)  == LOW;
  bool rightPressed = digitalRead(BTN_RIGHT) == LOW;

  State state = fsm.update(leftPressed, rightPressed);

  if (fsm.eventFinished()) {
    sdLogger.logSession(
        cfg,                        
        fsm.getLastStartTime(),
        fsm.getLastEndTime(),
        fsm.getLastEventButton().c_str()
    );
  }

  time_t now = time(nullptr);

  if (WiFi.status() == WL_CONNECTED &&
      difftime(now, lastSendTime) >= sendIntervalSec) {

    if (wifiSender.sendLogsEmail(cfg)) {
      sdLogger.clearLogs(cfg);
      lastSendTime = now;
    }
  }

}
