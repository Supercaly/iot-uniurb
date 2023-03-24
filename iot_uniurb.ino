#include "src/common.h"
#include "src/network/wifi.h"
#include "src/sensor/DHT11_sensor.h"

void setup() {
  Log.init(BAUD_RATE);

  // Connecto to WiFi network
  if (!wifi_connect(WIFI_SSID, WIFI_PWD, WIFI_MAX_CONN_RETRY, WIFI_RETRY_PAUSE_MS)) {
    Log.fatalln("something went wrong connecting to WiFi");
    reboot_board();
  }

  Log.infoln("SSID:               " + String(WIFI_SSID));
  Log.infoln("Device IP:          " + wifi_get_ip());

  // Init available sensors
  if (!DHT11_init()) {
    Log.errorln("something went wrong initializing DHT11");
  }
}

void loop() {
  // Read the available sensors
  DHT11_read();

  // Wait for some time before reading the sensors again
  delay(SENSOR_READING_DELAY_MS);
}
