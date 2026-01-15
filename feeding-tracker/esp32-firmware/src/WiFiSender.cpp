#include "../include/WiFiSender.h"

#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <SD.h>

#include "../config/config.h"

bool WiFiSender::connectWiFi(const Config& cfg) {

  if (cfg.wifi_ssid.length() == 0) {
    Serial.println("WiFi SSID is empty");
    return false;
  }

  Serial.print("Connecting to WiFi: ");
  Serial.println(cfg.wifi_ssid);

  WiFi.begin(cfg.wifi_ssid.c_str(),
             cfg.wifi_password.c_str());

  unsigned long startAttempt = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttempt < 10000) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("\nWiFi FAILED");
  return false;
}

bool WiFiSender::sendLogsEmail(const Config& cfg) {
  if (!SD.exists("/logs.csv")) return false;

  SMTPSession smtp;
  smtp.debug(1);

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;

  SMTP_Message message;
  message.sender.name = "ESP32 Logger";
  message.sender.email = AUTHOR_EMAIL;

  message.subject = "Breastfeeding Log";

  message.text.content = String("Hello, here is the latest breastfeeding log for ") +
                         cfg.mother_name + " and " + cfg.child_name + ". Please see attached CSV." ;

  message.addRecipient("Mom", cfg.email.c_str());

  SMTP_Attachment att;
  att.descr.filename = "logs.csv";
  att.descr.mime = "text/csv";
  att.file.path = "/logs.csv";
  att.file.storage_type = esp_mail_file_storage_type_sd;
  message.addAttachment(att);

  if (!smtp.connect(&session)) return false;
  if (!MailClient.sendMail(&smtp, &message)) return false;

  Serial.println("Email sent successfully!");
  return true;
}


