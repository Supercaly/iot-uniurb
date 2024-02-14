#include "ota.h"

#include <Arduino.h>
#include <ArduinoOTA.h>

#include "../config.h"
#include "../log.h"

static String ota_error_to_string(ota_error_t e) {
  switch (e) {
  case OTA_AUTH_ERROR:
    return "Auth Failed";
    break;
  case OTA_BEGIN_ERROR:
    return "Begin Failed";
    break;
  case OTA_CONNECT_ERROR:
    return "Connect Failed";
    break;
  case OTA_RECEIVE_ERROR:
    return "Receive Failed";
    break;
  case OTA_END_ERROR:
    return "End Failed";
    break;
  default:
    return "Unknown";
    break;
  }
}

bool ota_init(String host_name) {
  app_traceln("ota_init: initializing OTA");

  ArduinoOTA.setPort(OTA_PORT)
      .setHostname(host_name.c_str())
      .setPassword(OTA_PWD)
      .onStart([]() {
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        app_infoln("ota_init: onStart: updating " + (ArduinoOTA.getCommand() == U_FLASH)
                       ? "sketch"
                       : "filesystem");
      })
      .onEnd([]() { app_infoln("ota_init: onEnd: upload finished"); })
      .onProgress([](unsigned int progress, unsigned int total) {
        app_infoln("ota_init: onProgress: " + String(progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        app_error("ota_init: onError: error (" + ota_error_to_string(error) + ") ");
      });
  ArduinoOTA.begin();

  app_infoln("ota_init: init OTA server of port " + String(OTA_PORT));
  return true;
}

void ota_task_code(void *args) {
  TickType_t last_loop_time = xTaskGetTickCount();
  for (;;) {
    ArduinoOTA.handle();
    xTaskDelayUntil(&last_loop_time, pdMS_TO_TICKS(OTA_LOOP_DELAY_MS));
  }
}