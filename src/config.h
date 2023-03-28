#ifndef CONFIG_H
#define CONFIG_H

#include <assert.h>

/*
 * Global configuration header.
 * 
 * Here we put all the configurations for the
 * various elements of the project.
 */

// Enable Debug Mode.
#define IS_DEBUG

// Print the value of sensors after reading them.
#define PRINT_SENSORS_ON_READ

// Default serial used for print.
#define LOGGER_SERIAL Serial
// Serial Baud Rate.
#define BAUD_RATE 115200

// WiFi Settings
#define WIFI_SSID "SSID"
#define WIFI_PWD "pwd"

// Delay in milliseconds after each sensor reading loop.
#define SENSOR_READING_DELAY_MS 1000 * 60 * 5

// Delay in milliseconds before the board reboots.
#define BOARD_REBOOT_DELAY_MS 1000 * 60 * 5

// Size of the Average Window for sensors.
// NOTE: This value must be large than 2 since for each
// average window we remove the max and min values.
#define SENSOR_AVG_WINDOW 10
static_assert(SENSOR_AVG_WINDOW > 2, "SENSOR_AVG_WINDOW must be > 2");
// Time to wait before each sensor measurement when doing
// an averaged measurement.
#define SENSOR_AVG_DELAY_MS 300

// PIN of the LED used to signal to the user.
#define SIGNAL_LED_PIN 4

// PIN of the DHT11 sensor.
#define DHT11_PIN 15

// Serial for the MHZ19 sensor.
#define MHZ19_SERIAL Serial2
// Serial Baud Rate for the MHZ19 sensor.
#define MHZ19_BAUD_RATE 9600
// PINs of the MHZ19 sensor.
#define MHZ19_RX 16
#define MHZ19_TX 17

// InfluxDB settings.
#define HAS_INFLUXDB
#define INFLUXDB_URL "url"
#define INFLUXDB_ORG "org"
#define INFLUXDB_BUCKET "bucket"
#define INFLUXDB_TOKEN ""
#define INFLUXDB_POINT_NAME "point"

// Telnet settings.
#define HAS_TELNET

#endif  // CONFIG_H
