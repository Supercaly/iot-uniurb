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

// TODO: Make function set_mac_address publicly available.
//  Currently we call WiFi.mode(WIFI_STA) inside wifi_connect, so the WiFi
//  interface is not initialized until we try to connect to a network.
//  Using a public method to set the MAC address before connecting will always
//  result in errors since the WiFi interface is never initialize.
//  At the moment it's good setting the MAC privately only when connecting to a
//  network, but, in case we implement the access point we can let the user
//  change the MAC.
static void set_mac_address(String mac) {
  int     mac_val[6];
  uint8_t mac_bytes[6];

  // Parse MAC address string to bytes
  int sz = sscanf(mac.c_str(),
                  "%x:%x:%x:%x:%x:%x%*c",
                  &mac_val[0],
                  &mac_val[1],
                  &mac_val[2],
                  &mac_val[3],
                  &mac_val[4],
                  &mac_val[5]);
  if (sz != 6) {
    app_errorln("set_mac_address: invalid MAC address: '" + mac + "'");
    return;
  }

  for (int i = 0; i < 6; ++i) {
    mac_bytes[i] = (uint8_t)mac_val[i];
  }
  esp_err_t err = esp_wifi_set_mac(WIFI_IF_STA, mac_bytes);
  if (err == ESP_OK) {
    app_debugln("set_mac_address: new MAC address: " + wifi_get_mac_address());
  } else {
    app_errorln("set_mac_address: error setting MAC: " + String(esp_err_to_name(err)));
  }
}

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

  // If board has MAC address spoofing set it
  if (Preference.has_spoofed_mac()) {
    set_mac_address(Preference.get_spoofed_mac());
  }

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
