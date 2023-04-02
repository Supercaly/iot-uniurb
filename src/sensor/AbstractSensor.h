#ifndef ABSTRACT_SENSOR_H
#define ABSTRACT_SENSOR_H

/*
 * Class representing an abstract sensor that
 * is implemented by a real sensor.
 *
 * This class offers interfaces to initialize the 
 * sensor and measure his data.
 */
class AbstractSensor {
public:
  /*
   * Initialize the sensor.
   *
   * Returns true if the initialization is
   * successful, false otherwise.
   */
  virtual bool init() = 0;

  /*
   * Measure the values from the sensor.
   *
   * Returns true if the measure is successful,
   * false otherwise.
   */
  virtual bool measure() = 0;

protected:
  /*
   * Variable stating if the sensor is already initialized.
   */
  bool p_is_init = false;
};

#endif  // ABSTRACT_SENSOR_H
