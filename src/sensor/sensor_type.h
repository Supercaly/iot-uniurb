#ifndef SENSOR_TYPE_H
#define SENSOR_TYPE_H

#include <WString.h>
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

  COUNT_SENSORS
};

static_assert(COUNT_SENSORS == 5,
              "The number of Sensor Types have changed. "
              "You probably have added or removed a sensor. "
              "Please update the definitions above accordingly");

/*
 * Returns a String representation of given SensorType.
 */
String SensorType_to_String(SensorType t);

/*
 * Returns a SensorType form his name String.
 */
bool SensorType_by_name(String s, SensorType *t);

#endif // SENSOR_TYPE_H
