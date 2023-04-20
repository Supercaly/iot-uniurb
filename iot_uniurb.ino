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
    LOG_ERRORLN("something went wrong initializing board preferences");
  }

  LOG_INFOLN("Available Sensors:   " + Preference.available_sensors_to_String());
  LOG_INFOLN("Device Host Name:   '" + Preference.get_board_host_name() + "'");
  LOG_INFOLN("Device Location:    '" + Preference.get_board_location() + "'");
  LOG_INFOLN("Device Room:        '" + Preference.get_board_room() + "'");

#ifdef HAS_BACKUP_WIFI
  xTaskCreatePinnedToCore(wifi_backup_task_code,
                          "wifi_backup_task",
                          WIFI_BACKUP_TASK_STACK_SIZE,
                          nullptr,
                          WIFI_BACKUP_TASK_PRIORITY,
                          &wifi_backup_task_handler,
                          WIFI_BACKUP_TASK_CORE);
#endif // HAS_BACKUP_WIFI

  LOG_INFOLN("MAC Address:        '" + wifi_get_mac_address() + "'");
  LOG_INFOLN("SSID:               '" + String(WIFI_SSID) + "'");
  // Connecto to WiFi network
  if (!wifi_connect(WIFI_SSID, WIFI_PWD)) {
    LOG_FATALLN("something went wrong connecting to WiFi");
    reboot_board();
  }
  LOG_INFOLN("Device IP:          '" + wifi_get_ip() + "'");

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
  LOG_INFOLN("InfluxDB url:       '" INFLUXDB_URL "'");
  LOG_INFOLN("InfluxDB org:       '" INFLUXDB_ORG "'");
  LOG_INFOLN("InfluxDB bucket:    '" INFLUXDB_BUCKET "'");

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
