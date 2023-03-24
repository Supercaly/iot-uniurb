#include "utils.h"
#include "config.h"

void reboot_board() {
  LOGGER_SERIAL.println("rebooting the board...");
  delay(BOARD_REBOOT_DELAY_MS);
  ESP.restart();
}