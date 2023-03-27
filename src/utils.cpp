#include "utils.h"
#include "log.h"

void reboot_board() {
  Log.debugln("rebooting the board after "+String(BOARD_REBOOT_DELAY_MS)+"ms");
  delay(BOARD_REBOOT_DELAY_MS);
  Log.infoln("rebooting the board...");
  ESP.restart();
}