#ifndef OTA_H
#define OTA_H

// OTA task config.
#define OTA_TASK_STACK_SIZE 10240
#define OTA_TASK_PRIORITY   5
#define OTA_TASK_CORE       tskNO_AFFINITY
#define OTA_LOOP_DELAY_MS   250

/*
 * Init OTA server.
 */
bool ota_init();

/*
 * Function representing a task running inside the board
 * executing the OTA server loop.
 */
void ota_task_code(void *args);

#endif // OTA_H
