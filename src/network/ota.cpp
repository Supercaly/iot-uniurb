#include "ota.h"

#include <Arduino.h>
#include <ArduinoOTA.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"

bool ota_init() {
  app_traceln("ota_init: initializing OTA");
  BoardInfo bi;
  if (!Preference.get_board_info(&bi)) {
    app_errorln("ota_init: error retrieving board info");
    return false;
  }

  ArduinoOTA.setPort(OTA_PORT)
      .setHostname(bi.host_name.c_str())
      .setPassword(OTA_PWD)
      .onStart([]() {
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        app_infoln("ota_init: onStart: updating " + (ArduinoOTA.getCommand() == U_FLASH)
                       ? "sketch"
                       : "filesystem");
      })
      .onEnd([]() { app_infoln("ota_init: onEnd: upload finished"); })
      .onProgress([](unsigned int progress, unsigned int total) {
        app_info("ota_init: onProgress: " + String(progress / (total / 100)) + "\r");
      })
      .onError([](ota_error_t error) {
        app_error("ota_init: onError: error (" + String(error) + ") ");
        if (error == OTA_AUTH_ERROR) {
          app_errorln("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
          app_errorln("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
          app_errorln("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
          app_errorln("Receive Failed");
        } else if (error == OTA_END_ERROR) {
          app_errorln("End Failed");
        }
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