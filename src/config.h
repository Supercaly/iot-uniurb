#ifndef CONFIG_H
#define CONFIG_H

#include "../arduino_secrets.h"

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
#define WIFI_SSID SECRET_WIFI_SSID
#define WIFI_PWD  SECRET_WIFI_PWD

// #define HAS_BUTTON
#define BUTTON_PIN 13

// Delay in milliseconds after each sensor reading loop.
#define SENSOR_READING_DELAY_MS 1000 * 60 * 5

// Delay in milliseconds before the board reboots.
#define BOARD_REBOOT_DELAY_MS 1000 * 60 * 5

// PIN of the LED used to signal to the user.
#define SIGNAL_LED_PIN 4
// PIN of the DHT11 sensor.
#define DHT11_PIN 32
// PWM PIN for the MHZ19 sensor.
#define MHZ19B_PWM_PIN 16

// InfluxDB settings.
#define HAS_INFLUXDB
#define INFLUXDB_URL                       SECRET_INFLUXDB_URL
#define INFLUXDB_ORG                       SECRET_INFLUXDB_ORG
#define INFLUXDB_BUCKET                    SECRET_INFLUXDB_BUCKET
#define INFLUXDB_TOKEN                     SECRET_INFLUXDB_TOKEN
#define INFLUXDB_POINT_NAME                SECRET_INFLUXDB_POINT_NAME
#define INFLUXDB_FIELD_DHT11_TEMPERATURE   "dht11_temperature"
#define INFLUXDB_FIELD_DHT11_HUMIDITY      "dht11_humidity"
#define INFLUXDB_FIELD_SGP30_TVOC          "sgp30_TVOC"
#define INFLUXDB_FIELD_SGP30_ECO2          "sgp30_eCO2"
#define INFLUXDB_FIELD_SPS30_MC_1p0        "sps30_mc_1p0"
#define INFLUXDB_FIELD_SPS30_MC_2p5        "sps30_mc_2p5"
#define INFLUXDB_FIELD_SPS30_MC_4p0        "sps30_mc_4p0"
#define INFLUXDB_FIELD_SPS30_MC_10p0       "sps30_mc_10p0"
#define INFLUXDB_FIELD_SPS30_PARTICLE_SIZE "sps30_particle_size"
#define INFLUXDB_FIELD_MHZ19_CO2           "mhz19_co2"
#define INFLUXDB_FIELD_ENS160_TEMPERATURE  "aht2x_temperature"
#define INFLUXDB_FIELD_ENS160_HUMIDITY     "aht2x_humidity"
#define INFLUXDB_FIELD_ENS160_ECO2         "ens160_eco2"
#define INFLUXDB_FIELD_ENS160_TVOC         "ens160_tvoc"

// Telnet settings.
#define HAS_TELNET
#define TELNET_PORT 23

// OTA update settings.
#define HAS_OTA
#define OTA_PORT 3232
#define OTA_PWD  SECRET_OTA_PWD

#endif // CONFIG_H
