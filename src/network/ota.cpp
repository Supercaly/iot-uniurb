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
  app_traceln("ota_init: initializing OTA on port " + String(OTA_PORT));

  ArduinoOTA.setPort(OTA_PORT)
      .setHostname(host_name.c_str())
      .setPassword(OTA_PWD)
      .onStart([]() {
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        app_infoln("starting OTA update of " + (ArduinoOTA.getCommand() == U_FLASH) ? "sketch"
                                                                                    : "filesystem");
      })
      .onEnd([]() { app_infoln("OTA update finished"); })
      .onProgress([](unsigned int progress, unsigned int total) {
        app_infoln("OTA update: " + String(progress / (total / 100)) + "% (" + String(progress)
                   + "/" + String(total) + ")");
      })
      .onError([](ota_error_t error) {
        app_errorln("ota_init: onError: error performing OTA update (" + ota_error_to_string(error)
                    + ")");
      });
  ArduinoOTA.begin();

  app_traceln("ota_init: ota initialized");
  app_infoln("OTA on Port:        '" + String(OTA_PORT) + "'");
  return true;
}

void ota_task_code(void *args) {
  app_traceln("ota_task_code: starting task '" + String(OTA_TASK_NAME) + "'");
  TickType_t last_loop_time = xTaskGetTickCount();
  for (;;) {
    ArduinoOTA.handle();
    xTaskDelayUntil(&last_loop_time, pdMS_TO_TICKS(OTA_LOOP_DELAY_MS));
  }
}