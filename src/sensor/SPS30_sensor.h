#ifndef SPS30_SENSOR_H
#define SPS30_SENSOR_H

#define SPS30_MAX_PROBE_RETRY 10
#define SPS30_PROBE_RETRY_DELAY_MS 500
#define SPS30_FAN_AUTO_CLEAN_DAYS 4

// Delay in milliseconds for the sensor init process.
#define SPS30_INIT_DELAY_MS 300

/*
 * Init the sensor.
 */
bool SPS30_init();

/*
 * Read new values form the sensor.
 *
 * After calling this get the new values
 * using this getters:
 * - `SPS30_get_mc1p0`
 * - `SPS30_get_mc2p5`
 * - `SPS30_get_mc4p0`
 * - `SPS30_get_mc10p0`
 */
void SPS30_read();

/*
 * Returns the last measured MC 1.0
 */
float SPS30_get_mc1p0();
/*
 * Returns the last measured MC 2.5
 */
float SPS30_get_mc2p5();
/*
 * Returns the last measured MC 4.0
 */
float SPS30_get_mc4p0();
/*
 * Returns the last measured MC 10.0
 */
float SPS30_get_mc10p0();
/*
 * Returns the last measured NC 1.0
 */
float SPS30_get_nc1p0();
/*
 * Returns the last measured NC 2.5
 */
float SPS30_get_nc2p5();
/*
 * Returns the last measured NC 4.0
 */
float SPS30_get_nc4p0();
/*
 * Returns the last measured NC 10.0
 */
float SPS30_get_nc10p0();
/*
 * Returns the last measured typical particle size.
 */
float SPS30_get_particle_size();

#endif  // SPS30_SENSOR_H
