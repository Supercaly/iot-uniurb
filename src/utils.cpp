#include "utils.h"
#include "log.h"

void reboot_board() {
  Log.infoln("rebooting the board...");
  delay(BOARD_REBOOT_DELAY_MS);
  ESP.restart();
}