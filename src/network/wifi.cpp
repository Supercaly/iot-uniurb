#include "wifi.h"

#include <esp_wifi.h>
#include <WiFi.h>
#include "IPAddress.h"

#ifdef HAS_BACKUP_WIFI
static int old_backup_state = HIGH;
#endif  // HAS_BACKUP_WIFI

// TODO: Make function set_mac_address publicly available.
//  Currently we call WiFi.mode(WIFI_STA) inside wifi_connect, so the WiFi
//  interface is not initialized until we try to connect to a network.
//  Using a public method to set the MAC address before connecting will always
//  result in errors since the WiFi interface is never initialize.
//  At the moment it's good setting the MAC privately only when connecting to a
//  network, but, in case we implement the access piont we can let the user
//  change the MAC.
static void set_mac_address(String mac) {
  int mac_val[6];
  uint8_t mac_bytes[6];

  // Parse MAC address string to bytes
  int sz = sscanf(mac.c_str(), "%x:%x:%x:%x:%x:%x%*c",
                  &mac_val[0], &mac_val[1], &mac_val[2],
                  &mac_val[3], &mac_val[4], &mac_val[5]);
  if (sz != 6) {
    Log.errorln("set_mac_address: invalid MAC address: '" + mac + "'");
    return;
  }

  for (int i = 0; i < 6; ++i) {
    mac_bytes[i] = (uint8_t)mac_val[i];
  }
  esp_err_t err = esp_wifi_set_mac(WIFI_IF_STA, mac_bytes);
  if (err == ESP_OK) {
    Log.debugln("set_mac_address: new MAC address: "
                + wifi_get_mac_address());
  } else {
    Log.traceln("set_mac_address: error setting MAC: "
                + String(esp_err_to_name(err)));
  }
}

bool wifi_connect(const char *ssid, const char *pwd,
                  int max_retry, int pause) {
  Log.traceln("wifi_connect: connecting to SSID: '" + String(ssid)
              + "' with pwd: '" + String(pwd)
              + "' attempts: " + String(max_retry)
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

bool wifi_is_connected() {
  return (WiFi.status() == WL_CONNECTED);
}

String wifi_get_ip() {
  return WiFi.localIP().toString();
}

String wifi_get_mac_address() {
  return WiFi.macAddress();
}

#ifdef HAS_BACKUP_WIFI
void wifi_backup_task(void *args) {
  pinMode(WIFI_BACKUP_JUMPER_PIN, INPUT_PULLUP);
  digitalWrite(WIFI_BACKUP_JUMPER_PIN, HIGH);

  TickType_t last_check = xTaskGetTickCount();
  for (;;) {
    xTaskDelayUntil(&last_check, pdMS_TO_TICKS(1000));
    int backup_state = digitalRead(WIFI_BACKUP_JUMPER_PIN);
    // TODO: Switch the WiFi connection back once the jumper is unset.
    if (old_backup_state != backup_state && backup_state == LOW) {
      if (!wifi_connect(WIFI_BACKUP_SSID, WIFI_BACKUP_PWD)) {
        Log.errorln("wifi_backup_task: error connecting to backup WiFi");
        backup_state = HIGH;
      }
    }
    old_backup_state = backup_state;
  }
}
#endif  //HAS_BACKUP_WIFI
