#ifndef TELNET_H
#define TELNET_H

#include <Arduino.h>

#include "../config.h"

// Telent Task config.
#define TELNET_TASK_NAME       "telnet_task"
#define TELNET_TASK_STACK_SIZE 10240
#define TELNET_TASK_PRIORITY   5
#define TELNET_TASK_CORE       tskNO_AFFINITY

#define TELNET_LOOP_DELAY_MS 200

// Error message for sensors command unknown.
#define SENSORS_CMD_ERROR_STR(cmd)                       \
  "expecting a sensor name after " cmd "command. "       \
  "If you wan to to list all available sensors use the " \
  "sensors command instead or use help to view all commands."

/*
 * Init Telnet server.
 */
bool telnet_init();

/*
 * Function representing a task running inside the board
 * executing the Telnet server loop.
 */
void telnet_task_code(void *args);

#endif // TELNET_H
