#ifndef TELNET_H
#define TELNET_H

// Default telnet port
#define TELNET_PORT 23

// Telent Task config.
#define TELNET_TASK_STACK_SIZE 10240
#define TELNET_TASK_PRIORITY 0
#define TELNET_LOOP_DELAY_MS 200

/*
 * Init Telnet server.
 */
bool telnet_init(int port = TELNET_PORT);

/*
 * Run the Telnet server loop inside a task
 * with given core id.
 */
void telnet_run_on_core(int core_id);

#endif  // TELNET_H
