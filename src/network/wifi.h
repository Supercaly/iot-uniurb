#ifndef WIFI_H
#define WIFI_H

#include "../common.h"

// Maximum number of retry when connecting to WiFi.
#define WIFI_MAX_CONN_RETRY 10
// Delay between WiFi retries.
#define WIFI_RETRY_PAUSE_MS 2000

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
bool wifi_connect(const char *ssid, const char *pwd, int max_retry, int pause);

/*
 * Returns the current IP address as String if the device
 * is connected to a network, otherwise an empty IP address.
 */
String wifi_get_ip();

/*
 * Returns the board's MAC address as String.
 */
String wifi_get_mac_address();

#endif  // WIFI_H
