#include "src/common.h"
#include "src/network/wifi.h"
#ifdef HAS_INFLUXDB
#include "src/network/influxdb.h"
#endif  // HAS_INFLUXDB
#include "src/sensor/DHT11_sensor.h"
#include "src/sensor/SGP30_sensor.h"
#include "src/sensor/MHZ19_sensor.h"
#include "src/sensor/SPS30_sensor.h"

void setup() {
  Log.init(BAUD_RATE);

  Log.infoln("Device Host Name:   " HOST);
  Log.infoln("Device Location:    " LOG);
  Log.infoln("Device Room:        " ROOM);

  // Connecto to WiFi network
  if (!wifi_connect(WIFI_SSID, WIFI_PWD, WIFI_MAX_CONN_RETRY, WIFI_RETRY_PAUSE_MS)) {
    Log.fatalln("something went wrong connecting to WiFi");
    reboot_board();
  }

  Log.infoln("SSID:               " + String(WIFI_SSID));
  Log.infoln("Device IP:          " + wifi_get_ip());

#ifdef HAS_INFLUXDB
  // Init InfluxDB
  Log.infoln("InfluxDB url:       " INFLUXDB_URL);
  Log.infoln("InfluxDB org:       " INFLUXDB_ORG);
  Log.infoln("InfluxDB bucket:    " INFLUXDB_BUCKET);

  if (!influxdb_init()) {
    Log.fatalln("something went wrong initializing InfluxDB");
    reboot_board();
  }
#endif  // HAS_INFLUXDB

  // Init available sensors
  if (!DHT11_init()) {
    Log.errorln("something went wrong initializing DHT11");
  }
  if (!SGP30_init()) {
    Log.errorln("something went wrong initializing SGP30");
  }
  if (!MHZ19_init()) {
    Log.errorln("something went wrong initializing MHZ19");
  }
  if (!SPS30_init()) {
    Log.errorln("something went wrong initializing SPS30");
  }
}

void loop() {
  // Read the available sensors
  DHT11_read();
  SGP30_read();
  MHZ19_read();
  SPS30_read();

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
