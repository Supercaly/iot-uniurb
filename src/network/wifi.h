#ifndef WIFI_H
#define WIFI_H

#include "../common.h"

// Maximum number of retry when connecting to WiFi.
#define WIFI_MAX_CONN_RETRY 10
// Delay between WiFi retries.
#define WIFI_RETRY_PAUSE_MS 2000

// Backup WiFi task config.
#ifdef HAS_BACKUP_WIFI
// TODO: Reduce stack size used by wifi backup task
#define WIFI_BACKUP_TASK_STACK_SIZE 4096
#define WIFI_BACKUP_TASK_PRIORITY 5
#define WIFI_BACKUP_TASK_CORE tskNO_AFFINITY
#define WIFI_BACKUP_TASK_DELAY_MS 1000
#endif  // HAS_BACKUP_WIFI
/*
 * Returns true if the device is connected to a WiFi network,
 * false otherwise.
 */
bool wifi_is_connected();

/*
 * Connect the device to a WiFi network with given SSID and password.
 *
 * Return true if the connection is successful,
 * false in case of errors.
 */
bool wifi_connect(const char *ssid, const char *pwd,
                  int max_retry = WIFI_MAX_CONN_RETRY,
                  int pause = WIFI_RETRY_PAUSE_MS);

/*
 * Returns the current IP address as String if the device
 * is connected to a network, otherwise an empty IP address.
 */
String wifi_get_ip();

/*
 * Returns the board's MAC address as String.
 */
String wifi_get_mac_address();

/*
 * Function representing a task running on the board
 * with the purpose of siwtching the WiFi connection
 * from the default one to the backup.
 *
 * The backup WiFi connection is used in all those cases
 * where the default one is a public network with restricted
 * access to ports and we cannot connect to the board via
 * protocols; using a backup connection throug, for example,
 * an hotspot let us gain full access to the device.
 *
 * As soon the jumper is pulled to groun the backup WiFi
 * connection is enabled and it will remain so even when the
 * jumper is not connected anymore; to switch back to the default
 * connection it is required to disconnect the jumper and then
 * perform a full board reboot.
 */
#ifdef HAS_BACKUP_WIFI
void wifi_backup_task_code(void *args);
#endif  // HAS_BACKUP_WIFI

#endif  // WIFI_H
