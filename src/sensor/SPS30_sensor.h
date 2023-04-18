#ifndef SPS30_SENSOR_H
#define SPS30_SENSOR_H

#include <sps30.h>

#include "abstract_sensor.h"

#define SPS30_MAX_PROBE_RETRY      10
#define SPS30_PROBE_RETRY_DELAY_MS 500
#define SPS30_FAN_AUTO_CLEAN_DAYS  4

// Delay in milliseconds for the sensor init process.
#define SPS30_INIT_DELAY_MS 300

class SPS30_Sensor : public AbstractSensor {
  public:
  bool on_init() override;
  bool on_measure() override;

  /*
   * Returns the last measured MC 1.0
   */
  float get_mc_1p0() const { return _sps_meas.mc_1p0; }

  /*
   * Returns the last measured MC 2.5
   */
  float get_mc_2p5() const { return _sps_meas.mc_2p5; }

  /*
   * Returns the last measured MC 4.0
   */
  float get_mc_4p0() const { return _sps_meas.mc_4p0; }

  /*
   * Returns the last measured MC 10.0
   */
  float get_mc_10p0() const { return _sps_meas.mc_10p0; }

  /*
   * Returns the last measured NC 1.0
   */
  float get_nc_1p0() const { return _sps_meas.nc_1p0; }

  /*
   * Returns the last measured NC 2.5
   */
  float get_nc_2p5() const { return _sps_meas.nc_2p5; }

  /*
   * Returns the last measured NC 4.0
   */
  float get_nc_4p0() const { return _sps_meas.nc_4p0; }

  /*
   * Returns the last measured NC 10.0
   */
  float get_nc_10p0() const { return _sps_meas.nc_10p0; }

  /*
   * Returns the last measured typical particle size.
   */
  float get_particle_size() const { return _sps_meas.typical_particle_size; }

  private:
  struct sps30_measurement _sps_meas;
};

/*
 * Global instance of SPS30_Sensor.
 */
extern SPS30_Sensor SPS30Sensor;

#endif // SPS30_SENSOR_H
