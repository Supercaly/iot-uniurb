#ifndef TELNET_H
#define TELNET_H

#include "../common.h"

// Default telnet port
#define TELNET_PORT 23

// Telent Task config.
#define TELNET_TASK_STACK_SIZE 10240
#define TELNET_TASK_PRIORITY   5
#define TELNET_TASK_CORE       tskNO_AFFINITY

#define TELNET_LOOP_DELAY_MS 200

// Error message for sensors command unknown.
#define SENSORS_CMD_ERROR_STR(cmd)                      \
  "expecting a sensor name after " cmd "command. "      \
  "If you wanto to list all available sensors use the " \
  "sensors command instead or use help to view all commands."

/*
 * Init Telnet server.
 */
bool telnet_init(int port = TELNET_PORT);

/*
 * Function representing a task running inside the board
 * executing the Telnet server loop.
 */
void telnet_task_code(void *args);

#endif // TELNET_H
