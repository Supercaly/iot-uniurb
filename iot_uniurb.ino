#include "src/common.h"
#include "src/network/wifi.h"

void setup() {
  Log.init(BAUD_RATE);

  // Connecto to WiFi network
  if (!wifi_connect(WIFI_SSID, WIFI_PWD, WIFI_MAX_CONN_RETRY, WIFI_RETRY_PAUSE_MS)) {
    Log.fatalln("something went wrong connecting to WiFi");
    reboot_board();
  }

  Log.infoln("SSID:               " + String(WIFI_SSID));
  Log.infoln("Device IP:          " + wifi_get_ip());
}

void loop() {
}
