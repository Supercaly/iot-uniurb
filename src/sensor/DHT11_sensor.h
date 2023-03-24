#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H

// Delay in milliseconds for the sensor init process.
#define DHT11_INIT_DELAY_MS 300

/*
 * Init the DHT11 sensor.
 */
bool DHT11_init();

/*
 * Read data from the DHT11 sensor.
 *
 * After calling this method the data will be
 * available using the getters:
 * - `DHT11_get_temperature`
 * - `DHT11_get_humidity`
 */
void DHT11_read();

/*
 * Return the last measured temperature.
 */
float DHT11_get_temperature();

/*
 * Return the last measured humidity.
 */
float DHT11_get_humidity();

#endif  // DHT11_SENSOR_H
