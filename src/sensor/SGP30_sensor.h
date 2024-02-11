#ifndef SGP30_SENSOR_H
#define SGP30_SENSOR_H

#include <Adafruit_SGP30.h>
#include <stdint.h>

#include "sensor.h"

// Delay in milliseconds for the init process.
#define SGP30_INIT_DELAY_MS 300
// Delay in milliseconds after each sensor read.
#define SGP30_SAMPLING_MS 300
// Maximum value accepted for the eCO2.
#define SGP30_MAX_ECO2 60000
// Maximum value accepted for the TVOC.
#define SGP30_MAX_TVOC 60000
// Number of iterations used to compute the mean eCO2 and TVOC.
#define SGP30_AVG_NUM 10

class SGP30_Sensor : public AbstractSensor {
  public:
  bool on_init() override;
  bool on_measure() override;
  void print_info(sensor_print_cb_t) override;
  void to_influx(Point *) override;

  /*
   * Return the last measured Total Volatile Organic
   * Compounds in ppb.
   */
  int get_TVOC() const { return _tvoc; }

  /*
   * Return the last measured equivalent CO2 in ppm.
   */
  int get_eCO2() const { return _eco2; }

  private:
  Adafruit_SGP30 _sgp;
  uint16_t       _tvoc = 0;
  uint16_t       _eco2 = 0;
};

/*
 * Global instance of SGP30_Sensor
 */
extern SGP30_Sensor SGP30Sensor;

#endif // SGP30_SENSOR_H
