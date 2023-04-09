#include "src/common.h"
#include "src/network/wifi.h"
#ifdef HAS_INFLUXDB
#include "src/network/influxdb.h"
#endif  // HAS_INFLUXDB
#ifdef HAS_TELNET
#include "src/network/telnet.h"
#endif  // HAS_TELNET
#include "src/sensor_helper.h"

TaskHandle_t measure_task_handler;
#ifdef HAS_TELNET
TaskHandle_t telnet_task_handler;
#endif  // HAS_TELNET

void setup() {
  // Init logger
  Log.init(BAUD_RATE);

  // Init preferences
  if (!preferences_init()) {
    Log.errorln("something went wrong initializing board preferences");
  }

  Log.infoln("Available Sensors:   " + available_sensors_to_String());
  Log.infoln("Device Host Name:   '" + get_board_host_name() + "'");
  Log.infoln("Device Location:    '" + get_board_location() + "'");
  Log.infoln("Device Room:        '" + get_board_room() + "'");

  Log.infoln("MAC Address:        '" + wifi_get_mac_address() + "'");
  Log.infoln("SSID:               '" + String(WIFI_SSID) + "'");
  // Connecto to WiFi network
  if (!wifi_connect(WIFI_SSID, WIFI_PWD)) {
    Log.fatalln("something went wrong connecting to WiFi");
    reboot_board();
  }
  Log.infoln("Device IP:          '" + wifi_get_ip() + "'");

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
    Log.errorln("something went wrong initializing Telnet");
  }
#endif  // HAS_TELNET

#ifdef HAS_INFLUXDB
  // Init InfluxDB
  Log.infoln("InfluxDB url:       '" INFLUXDB_URL "'");
  Log.infoln("InfluxDB org:       '" INFLUXDB_ORG "'");
  Log.infoln("InfluxDB bucket:    '" INFLUXDB_BUCKET "'");

  if (!influxdb_init()) {
    Log.fatalln("something went wrong initializing InfluxDB");
    reboot_board();
  }
#endif  // HAS_INFLUXDB

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
