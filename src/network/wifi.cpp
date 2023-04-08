#include "wifi.h"

#include <WiFi.h>
#include "IPAddress.h"

bool wifi_is_connected() {
  return (WiFi.status() == WL_CONNECTED);
}

bool wifi_connect(const char *ssid, const char *pwd,
                  int max_retry, int pause) {
  Log.traceln("wifi_connect: connecting to SSID: " + String(ssid)
              + " with pwd: " + String(pwd)
              + " attempt: " + String(max_retry)
              + " delay: " + String(pause));

  LED_ON(SIGNAL_LED_PIN);

  // Remove previous connection
  WiFi.disconnect();

  // Enable WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pwd);
  Log.debug("wifi_connect: connecing to WiFi");

  // Test the connection for some times
  int conn_attempt = 0;
  while (!wifi_is_connected() && conn_attempt < max_retry) {
    Log.debug(".");
    delay(pause);
    conn_attempt++;
  }

  // If we are not connected return failure
  if (conn_attempt >= max_retry) {
    Log.debugln("failed");
    return false;
  }

  // Connection done
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Log.debugln("done");

  LED_OFF(SIGNAL_LED_PIN);
  return true;
}

String wifi_get_ip() {
  return WiFi.localIP().toString();
}

String wifi_get_mac_address() {
  return WiFi.macAddress();
}
