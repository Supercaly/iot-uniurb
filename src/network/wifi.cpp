#include "wifi.h"

#include <Arduino.h>
#include <IPAddress.h>
#include <WString.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <stdint.h>
#include <stdio.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"
#include "../utils.h"

bool wifi_connect(const char *ssid, const char *pwd, int max_retry, int pause) {
  app_traceln("wifi_connect: connecting to SSID: '" + String(ssid) + "' with pwd: '" + String(pwd)
              + "' attempts: " + String(max_retry) + " delay: " + String(pause));

  app_infoln("SSID:               '" + String(WIFI_SSID) + "'");
  app_infoln("MAC Address:        '" + wifi_get_mac_address() + "'");

  LED_ON(SIGNAL_LED_PIN);

  // Remove previous connection
  WiFi.disconnect();

  // Enable WiFi
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, pwd);
  app_debug("wifi_connect: connecting to WiFi");

  // Test the connection for some times
  int conn_attempt = 0;
  while (!wifi_is_connected() && conn_attempt < max_retry) {
    app_debug(".");
    delay(pause);
    conn_attempt++;
  }

  // If we are not connected return failure
  if (conn_attempt >= max_retry) {
    app_debugln("failed");
    return false;
  }

  // Connection done
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  app_debugln("done");

  app_infoln("Device IP:          '" + wifi_get_ip() + "'");

  LED_OFF(SIGNAL_LED_PIN);
  return true;
}

bool wifi_is_connected() {
  return (WiFi.status() == WL_CONNECTED);
}

String wifi_get_ip() {
  return WiFi.localIP().toString();
}

String wifi_get_mac_address() {
  return WiFi.macAddress();
}
