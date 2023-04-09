#include "src/common.h"
#include "src/network/wifi.h"
#ifdef HAS_INFLUXDB
#include "src/network/influxdb.h"
#endif  // HAS_INFLUXDB
#ifdef HAS_TELNET
#include "src/network/telnet.h"
#endif  // HAS_TELNET
#include "src/sensor_helper.h"

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
  if (!telnet_init()) {
    Log.errorln("something went wrong initializing Telnet");
  }
  telnet_run_on_core(0);
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
}

void loop() {
  // Read the available sensors
  measure_all_available_sensors();

#ifdef HAS_INFLUXDB
  // Send sensor values to InfluxDB
  if (!influxdb_write_sensors()) {
    Log.fatalln("something went wrong writing to InfluxDB");
    reboot_board();
  }
#endif  // HAS_INFLUXDB

  // Wait for some time before reading the sensors again
  delay(SENSOR_READING_DELAY_MS);
}
