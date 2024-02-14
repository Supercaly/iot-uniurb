#include <Arduino.h>
#include <esp_ota_ops.h>

#include "src/board_preference.h"
#include "src/config.h"
#include "src/log.h"
#include "src/network/wifi.h"
#include "src/sensor_helper.h"
#ifdef HAS_INFLUXDB
#  include "src/network/influxdb.h"
#endif // HAS_INFLUXDB
#ifdef HAS_BUTTON
#  include "src/button.h"
#endif // HAS_BUTTON
#ifdef HAS_TELNET
#  include "src/network/telnet.h"
#endif // HAS_TELNET
#ifdef HAS_OTA
#  include "src/network/ota.h"
#endif // HAS_OTA

TaskHandle_t measure_task_handler;
#ifdef HAS_BUTTON
TaskHandle_t button_task_handler;
#endif // HAS_BUTTON
#ifdef HAS_TELNET
TaskHandle_t telnet_task_handler;
#endif // HAS_TELNET
#ifdef HAS_OTA
TaskHandle_t ota_task_handler;
#endif // HAS_OTA

void setup() {
  // Init logger
  logger_init();

  // Init preferences
  if (!Preference.init()) {
    app_fatalln("something went wrong initializing board preferences");
  }

  // Print name of boot partition
  const esp_partition_t *boot_partition = esp_ota_get_running_partition();
  app_infoln("Boot Partition:     '" + String(boot_partition->label) + "'");

  // Connect to WiFi network
  if (!wifi_connect(WIFI_SSID, WIFI_PWD)) {
    app_fatalln("something went wrong connecting to WiFi");
    reboot_board();
  }

#ifdef HAS_BUTTON
  // Init button
  xTaskCreatePinnedToCore(button_task_code,
                          "button_task",
                          BUTTON_TASK_STACK_SIZE,
                          nullptr,
                          BUTTON_TASK_PRIORITY,
                          &button_task_handler,
                          BUTTON_TASK_CORE);
#endif // HAS_BUTTON

#ifdef HAS_TELNET
  // Init telnet
  if (telnet_init()) {
    xTaskCreatePinnedToCore(telnet_task_code,
                            "telnet_task",
                            TELNET_TASK_STACK_SIZE,
                            nullptr,
                            TELNET_TASK_PRIORITY,
                            &telnet_task_handler,
                            TELNET_TASK_CORE);
  } else {
    app_errorln("something went wrong initializing Telnet");
  }
#endif // HAS_TELNET

#ifdef HAS_OTA
  // Init OTA updates
  BoardInfo bi;
  Preference.get_board_info(&bi);
  if (bi.ota_enabled) {
    if (ota_init(bi.host_name)) {
      xTaskCreatePinnedToCore(ota_task_code,
                              "ota_task",
                              OTA_TASK_STACK_SIZE,
                              nullptr,
                              OTA_TASK_PRIORITY,
                              &ota_task_handler,
                              OTA_TASK_CORE);
    } else {
      app_errorln("something went wrong initializing OTA updates");
    }
  } else {
    app_infoln("OTA updates disabled");
  }
#endif // HAS_OTA

#ifdef HAS_INFLUXDB
  // Init InfluxDB
  if (!influxdb_init()) {
    app_fatalln("something went wrong initializing InfluxDB");
  }
#endif // HAS_INFLUXDB

  // Init available sensors
  init_available_sensors();

  xTaskCreatePinnedToCore(measure_and_send_task_code,
                          "measure_task",
                          MEASURE_TASK_STACK_SIZE,
                          nullptr,
                          MEASURE_TASK_PRIORITY,
                          &measure_task_handler,
                          MEASURE_TASK_CORE);
}

void loop() {}
