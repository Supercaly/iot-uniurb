#include "wifi.h"

#include <WiFi.h>
#include "IPAddress.h"

#ifdef HAS_BACKUP_WIFI
static int old_backup_state = HIGH;
#endif  // HAS_BACKUP_WIFI

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
