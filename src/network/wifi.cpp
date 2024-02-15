#include "wifi.h"

#include <Arduino.h>
#include <IPAddress.h>
#include <WiFi.h>
#include <esp_wifi.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"
#include "../utils.h"

bool wifi_connect() {
  app_traceln("wifi_connect: connecting WiFi with SSID: '" + String(WIFI_SSID) + "'");
  app_infoln("SSID:               '" + String(WIFI_SSID) + "'");
  app_infoln("MAC Address:        '" + wifi_get_mac_address() + "'");

  LED_ON(SIGNAL_LED_PIN);

  // Remove previous connection
  WiFi.disconnect();

  // Enable WiFi
  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WIFI_PWD);
  app_debug("Connecting WiFi");

  // Test the connection for some times
  int conn_attempt = 0;
  while (!wifi_is_connected() && conn_attempt < WIFI_MAX_CONN_RETRY) {
    app_debug(".");
    delay(WIFI_RETRY_PAUSE_MS);
    conn_attempt++;
  }

  // If we are not connected return failure
  if (conn_attempt >= WIFI_MAX_CONN_RETRY) {
    app_debugln(" FAILED");
    return false;
  }

  // Connection done
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  app_debugln(" DONE");

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
