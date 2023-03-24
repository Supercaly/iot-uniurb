#ifndef MHZ19_SENSOR_H
#define MHZ19_SENSOR_H

// Delay in milliseconds for the sensor init process.
#define MHZ19_INIT_DELAY_MS 300

/*
 * Init the MHZ19 sensor.
 */
bool MHZ19_init();

/*
 * Read new values from the sensor.
 *
 * After calling this method get the
 * values using the getters:
 * - `MHZ19_get_co2`
 * - `MHZ19_get_minCo2`
 * - `MHZ19_get_temperature`
 * - `MHZ19_get_accuracy`
 */
void MHZ19_read();

/*
 * Returns the last measured CO2.
 */
int MHZ19_get_co2();

/*
 * Returns the last measured minimum CO2.
 */
int MHZ19_get_minCo2();

/*
 * Returns the last measured temperature.
 */
int MHZ19_get_temperature();

/*
 * Returns the last measured accuracy.
 */
int MHZ19_get_accuracy();

#endif  // MHZ19_SENSOR_H
