#include <Arduino.h>
#include <WString.h>

#include "src/board_preference.h"
#include "src/config.h"
#include "src/log.h"
#include "src/network/wifi.h"
#include "src/sensor_helper.h"
#ifdef HAS_INFLUXDB
#  include "src/network/influxdb.h"
#endif // HAS_INFLUXDB
#ifdef HAS_TELNET
#  include "src/network/telnet.h"
#endif // HAS_TELNET

TaskHandle_t measure_task_handler;
#ifdef HAS_BACKUP_WIFI
TaskHandle_t wifi_backup_task_handler;
#endif // HAS_BACKUP_WIFI
#ifdef HAS_TELNET
TaskHandle_t telnet_task_handler;
#endif // HAS_TELNET

void setup() {
  // Init logger
  logger_init();

  // Init preferences
  if (!Preference.init()) {
    LOG_FATALLN("something went wrong initializing board preferences");
  }

#ifdef HAS_BACKUP_WIFI
  xTaskCreatePinnedToCore(wifi_backup_task_code,
                          "wifi_backup_task",
                          WIFI_BACKUP_TASK_STACK_SIZE,
                          nullptr,
                          WIFI_BACKUP_TASK_PRIORITY,
                          &wifi_backup_task_handler,
                          WIFI_BACKUP_TASK_CORE);
#endif // HAS_BACKUP_WIFI

  // Connecto to WiFi network
  if (!wifi_connect(WIFI_SSID, WIFI_PWD)) {
    LOG_FATALLN("something went wrong connecting to WiFi");
    reboot_board();
  }

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
    LOG_ERRORLN("something went wrong initializing Telnet");
  }
#endif // HAS_TELNET

#ifdef HAS_INFLUXDB
  // Init InfluxDB
  if (!influxdb_init()) {
    LOG_FATALLN("something went wrong initializing InfluxDB");
  }
#endif // HAS_INFLUXDB

  // Init available sensors
  init_all_available_sensors();

  xTaskCreatePinnedToCore(measure_and_send_task_code,
                          "measure_task",
                          MEASURE_TASK_STACK_SIZE,
                          nullptr,
                          MEASURE_TASK_PRIORITY,
                          &measure_task_handler,
                          MEASURE_TASK_CORE);
}

void loop() {}
