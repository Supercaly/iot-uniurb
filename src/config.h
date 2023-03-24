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

#endif  // CONFIG_H
