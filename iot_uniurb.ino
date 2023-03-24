#include "src/common.h"
#include "src/network/wifi.h"

void setup() {
  LOGGER_SERIAL.begin(BAUD_RATE);

  // Connecto to WiFi network
  if (!wifi_connect(WIFI_SSID, WIFI_PWD, WIFI_MAX_CONN_RETRY, WIFI_RETRY_PAUSE_MS)) {
    LOGGER_SERIAL.println("something went wrong connecting to WiFi");
  }

  LOGGER_SERIAL.println("SSID:               " + String(WIFI_SSID));
  LOGGER_SERIAL.println("Device IP:          " + wifi_get_ip().toString());
}

void loop() {
}
