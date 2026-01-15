#include "../include/SDLogger.h"
#include "../config/config.h" 

void SDLogger::init(const Config& cfg, TimeManager& tm) {
  timeMgr = &tm;
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("SD init FAILED");
    while(true);
  }
  Serial.println("SD init OK");

  if (!SD.exists(logFile)) {
    File f = SD.open(logFile, FILE_WRITE);
    if (f) {
      f.print("header,mother_name,child_name,pdr,dob,start_time,end_time,duration_sec,breast,comment");
      f.println();
      f.close();
    }
  }
}

String SDLogger::formatTime(time_t t) {
  struct tm tm;
  localtime_r(&t, &tm);
  char buf[25];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
  return String(buf);
}

void SDLogger::logSession(const Config& cfg, time_t startT, time_t endT,
                          String breast) {
  File f = SD.open(logFile, FILE_APPEND);
  if (!f) return;

  long duration = difftime(endT, startT);

  f.print("Breastfeeding Log"); f.print(",");
  f.print(cfg.mother_name); f.print(",");
  f.print(cfg.child_name); f.print(",");
  f.print(cfg.pdr); f.print(",");
  f.print(cfg.dob); f.print(",");
  f.print(formatTime(startT)); f.print(",");
  f.print(formatTime(endT)); f.print(",");
  f.print(duration); f.print(",");
  f.print(breast); f.print(",");
  f.println("");

  f.close();

  Serial.print("LOG: "); Serial.print(breast);
  Serial.print(" | "); Serial.print(formatTime(startT));
  Serial.print(" -> "); Serial.print(formatTime(endT));
  Serial.print(" ("); Serial.print(duration); Serial.println(" sec)");
}

void SDLogger::clearLogs(const Config& cfg) {
  SD.remove(logFile);
  File f = SD.open(logFile, FILE_WRITE);
  if (f) {
    f.print("header,mother_name,child_name,pdr,dob,start_time,end_time,duration_sec,breast,comment");
    f.println();
    f.close();
  }
  Serial.println("Logs cleared");
}
