#ifndef SGP30_SENSOR_H
#define SGP30_SENSOR_H

#include <Adafruit_SGP30.h>
#include <stdint.h>

#include "abstract_sensor.h"

// Delay in milliseconds for the init process.
#define SGP30_INIT_DELAY_MS 300
// Delay in milliseconds before measuring the sensor.
#define SGP30_MEASURE_DELAY_MS 1000

class SGP30_Sensor : public AbstractSensor {
  public:
  bool on_init() override;
  bool on_measure() override;

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
  /*
   * Return the absolute humidity from given temperature and humidity.
   */
  static uint32_t get_absolute_humidity(float temperature, float humidity);

  Adafruit_SGP30 _sgp;
  uint16_t       _tvoc = 0.0;
  uint16_t       _eco2 = 0.0;
};

/*
 * Global instance of SGP30_Sensor
 */
extern SGP30_Sensor SGP30Sensor;

#endif // SGP30_SENSOR_H
