#include "sensor.h"

#include <Arduino.h>
#include <assert.h>

#include "../utils.h"

static String sensor_type_to_string_map[]
    = {"SENSOR_DHT11", "SENSOR_SGP30", "SENSOR_MHZ19", "SENSOR_SPS30", "SENSOR_ENS160"};
static_assert(size_of_array(sensor_type_to_string_map) == SensorType::COUNT_SENSORS,
              "The number of elements of sensor_type_to_string_map have changed. "
              "You probably have added or removed a sensor. "
              "Please update the definitions above accordingly");

String sensor_type_to_string(SensorType t) {
  return sensor_type_to_string_map[t];
}

bool sensor_type_by_name(String s, SensorType *t) {
  if (s == "DHT11") {
    *t = SensorType::SENSOR_DHT11;
    return true;
  } else if (s == "SGP30") {
    *t = SensorType::SENSOR_SGP30;
    return true;
  } else if (s == "SPS30") {
    *t = SensorType::SENSOR_SPS30;
    return true;
  } else if (s == "MHZ19") {
    *t = SensorType::SENSOR_MHZ19;
    return true;
  } else if (s == "ENS160") {
    *t = SensorType::SENSOR_ENS160;
    return true;
  } else {
    return false;
  }
}

bool AbstractSensor::init() {
  if (p_is_init) {
    return true;
  }

  bool init_res = on_init();
  p_is_init     = init_res;
  return init_res;
}

bool AbstractSensor::measure() {
  if (!p_is_init) {
    return false;
  }

  return on_measure();
}
