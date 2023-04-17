#include "utils.h"

#include <esp_timer.h>

#include "log.h"

void reboot_board(int wait_ms) {
  Log.debugln("rebooting the board after " + String(wait_ms) + "ms");
  delay(wait_ms);
  Log.infoln("rebooting the board...");
  ESP.restart();
}

String board_uptime() {
  int64_t micro = esp_timer_get_time();

  int64_t seconds = micro / 1000000;
  int64_t minutes = seconds / 60;
  int64_t hours   = minutes / 60;
  int64_t days    = hours / 24;

  return String(days) + "days, " + String(hours % 24) + "hours, " + String(minutes % 60)
         + "minutes, " + String(seconds % 60) + "seconds";
}
