#ifndef ABSTRACT_SENSOR_H
#define ABSTRACT_SENSOR_H

#include <Arduino.h>
#include <InfluxDbClient.h>
#include <assert.h>

/*
 * Enum representing the type of
 * sensors available to the board.
 */
enum SensorType {
  SENSOR_DHT11 = 0,
  SENSOR_SGP30,
  SENSOR_MHZ19,
  SENSOR_SPS30,
  SENSOR_ENS160,
  // Count the number of sensors
  COUNT_SENSORS
};

static_assert(COUNT_SENSORS == 5,
              "The number of elements of SensorType have changed. "
              "You probably have added or removed a sensor. "
              "Please update the definition above accordingly.");

/*
 * Returns a String representation of given SensorType.
 */
String sensor_type_to_string(SensorType t);

/*
 * Returns a SensorType form his string name.
 *
 * This function returns true if the string has
 * an associated SensorType; false otherwise.
 * The SensorType is returned as a pointer passed
 * with the parameters.
 */
bool sensor_type_by_name(String s, SensorType *t);

/*
 * Callback function used to print sensor info.
 */
typedef void (*sensor_print_cb_t)(String);

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

  /*
   * Interface method implemented by the child class
   * to print his sensor infos.
   */
  virtual void print_info(sensor_print_cb_t) = 0;

  /*
   * Interface method implemented by the child class
   * to append to the given influxdb point his sensor data.
   */
  virtual void to_influx(Point *) = 0;

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
