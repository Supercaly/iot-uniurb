#ifndef SENSOR_HELPER_H
#define SENSOR_HELPER_H

#include "sensor_type.h"
#include "abstract_sensor.h"
#include "../board_preferences.h"
#include "../common.h"
#include "DHT11_sensor.h"
#include "SGP30_sensor.h"
#include "SPS30_sensor.h"
#include "MHZ19_sensor.h"

/*
 * Struct that maps SensorType to objects
 * implementing AbstractSensor.
 */
struct SensorTypeToImplPair {
  SensorType type;
  AbstractSensor *sensor;
};

static const SensorTypeToImplPair type_to_sensor_map[] = {
  { SensorType::SENSOR_DHT11, &DHT11Sensor },
  { SensorType::SENSOR_SGP30, &SGP30Sensor },
  { SensorType::SENSOR_MHZ19, &MHZ19Sensor },
  { SensorType::SENSOR_SPS30, &SPS30Sensor },
};
static_assert(size_of_array(type_to_sensor_map) == 4,
              "The number of elements of type_to_sensor_map have changed. "
              "You probably have added or removed a sensor. "
              "Please update the definitions above accordingly");

/*
 * Helper method to automatically init all sensors
 * if they are available.
 *
 * Returns true if all available sensors are initialized,
 * false if at least one available sensor cannot be initialized.
 */
bool init_all_available_sensors() {
  bool has_errors = false;
  for (int i = 0; i < size_of_array(type_to_sensor_map); i++) {
    SensorTypeToImplPair p = type_to_sensor_map[i];
    if (has_sensor(p.type)) {
      if (p.sensor == nullptr || !p.sensor->init()) {
        Log.errorln("something went wrong initializing "
                    + SensorType_to_String(p.type));
        has_errors = true;
      }
    }
  }
  return has_errors;
}

/*
 * Helper method to automatically measure all available sensors.
 *
 * Returns true if all available sensors are measured, false
 * if at least one available sensor cannot be measured.
 */
bool measure_all_available_sensors() {
  bool has_errors = false;
  for (int i = 0; i < size_of_array(type_to_sensor_map); i++) {
    SensorTypeToImplPair p = type_to_sensor_map[i];
    if (has_sensor(p.type)) {
      if (p.sensor == nullptr || !p.sensor->measure()) {
        Log.errorln("something went wrong measuring "
                    + SensorType_to_String(p.type));
        has_errors = true;
      }
    }
  }
  return has_errors;
}

#endif  // SENSOR_HELPER_H
