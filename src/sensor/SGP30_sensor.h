#ifndef SGP30_SENSOR_H
#define SGP30_SENSOR_H

// Delay in milliseconds for the init process.
#define SGP30_INIT_DELAY_MS 300

/*
 * Init the SGP30 sensor.
 */
bool SGP30_init();

/*
 * Read data from the SGP30 sensor.
 *
 * After calling this method the data will be
 * available using the getters:
 * - `SGP30_get_TVOC`
 * - `SGP30_get_eCO2`
 * - `SGP30_get_raw_H2`
 * - `SGP30_get_raw_ethanol`
 */
void SGP30_read();

/*
 * Return the last measured Total Volatile
 * Organic Compounds in ppb.
 */
int SGP30_get_TVOC();

/*
 * Return the last measured equivalent CO2 in ppm.
 */
int SGP30_get_eCO2();

/*
 * Return the last measured raw H2.
 */
int SGP30_get_raw_H2();

/*
 * Return the last measured raw Ethanol.
 */
int SGP30_get_raw_ethanol();

#endif  // SGP30_SENSOR_H
