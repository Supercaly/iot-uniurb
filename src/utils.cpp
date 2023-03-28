#include "utils.h"
#include "log.h"

void reboot_board(int wait_ms) {
  Log.debugln("rebooting the board after " + String(wait_ms) + "ms");
  delay(wait_ms);
  Log.infoln("rebooting the board...");
  ESP.restart();
}