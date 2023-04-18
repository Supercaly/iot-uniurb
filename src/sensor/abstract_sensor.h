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
  bool init();

  /*
   * Measure the values from the sensor.
   *
   * Returns true if the measure is successful,
   * false otherwise.
   */
  bool measure();

  protected:
  /*
   * Variable stating if the sensor is already initialized.
   */
  bool p_is_init = false;

  /*
   * Interface method implemented by the child class
   * to perform the init of his sensor.
   */
  virtual bool on_init() = 0;

  /*
   * Interface method implemented by the child class
   * to perform a measure of his sensor.
   */
  virtual bool on_measure() = 0;
};

#endif // ABSTRACT_SENSOR_H
